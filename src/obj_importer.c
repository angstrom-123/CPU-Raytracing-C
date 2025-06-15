#include "obj_importer.h"

/*
 * PRIVATE:
 */

static enum _E_Parse_Type {
	_UINT32_TRIPLE,
	_VECTOR
} _E_Parse_Type;

static const size_t _max_token_chars = 5;
static const size_t _max_line_tokens = 10;

static char** _alloc_str_arr()
{
	char** tokens;
	if ((tokens = malloc(_max_line_tokens * sizeof(char*))) == NULL)
	{
		fprintf(stderr, "malloc failed in obj importer");
		exit(1);
	}

	for (size_t i = 0; i < _max_line_tokens; i++)
	{
		tokens[i] = malloc(_max_token_chars * sizeof(char) + 1);
		if (tokens[i] == NULL)
		{
			fprintf(stderr, "malloc failed in obj importer");
			exit(1);
		}
	}

	return tokens;
}

static Token_Line* _tokenize(char* str, char delims[], size_t delim_len)
{
	Token_Line* line;
	if ((line = malloc(sizeof(Token_Line))) == NULL)
	{
		fprintf(stderr, "malloc failed in obj importer");
		exit(1);
	}

	line->tokens = _alloc_str_arr();
	line->length = 0;

	char* buf;
	if ((buf = malloc(_max_token_chars * sizeof(char) + 1)) == NULL)
	{
		fprintf(stderr, "malloc failed in obj importer");
		exit(1);
	}
	buf[0] = '\0'; // init with null terminator

	char c;
	size_t ctr = 0;
	while ((c = str[ctr++]) != '\0') // input string is null terminated
	{
		bool is_delim = false;
		for (size_t i = 0; (i < delim_len) && !is_delim; i++)
			is_delim = (c == delims[i]);

		if (is_delim) 
		{
			// flush and clear buffer
			memcpy(*(line->tokens + line->length++), buf, _max_token_chars);
			memset(buf, 0, _max_token_chars * sizeof(char) + 1);
			buf[0] = '\0';
		} 
		else 
		{
			// add current character to buffer
			size_t buf_head = 0;
			while (buf[buf_head++] != '\0');
			buf_head--;
			if (buf_head >= _max_token_chars)
			{
				fprintf(stderr, "buffer overrun in tokenize\n");
				exit(1);
			}
			buf[buf_head] = c;
			buf[buf_head + 1] = '\0';
		}
	}

	free(buf);
	return line;
}

static Token_Line** _get_lines_with_prefix(FILE* file, size_t* end_ptr, char* prefix)
{
	Token_Line** toks;
	size_t toks_head = 0;
	if ((toks = malloc(100 * sizeof(Token_Line*))) == NULL)
	{
		fprintf(stderr, "malloc failed in obj importer\n");
		exit(1);
	}

	char delims[] = {'\n', '\r', ' '};
	char read_buf[30];
	fseek(file, 0, SEEK_SET);
	while (fgets(read_buf, sizeof(read_buf) / sizeof(read_buf[0]), file))
	{
		Token_Line* token_arr = _tokenize(read_buf, delims, 3);
		if (token_arr->length == 0) // ignore empty lines
			continue;

		if (strcmp(token_arr->tokens[0], prefix) == 0) // matches
		{
			toks[toks_head++] = token_arr;
			*end_ptr = *end_ptr + 1;
		}
	}
	return toks;
}

static void* _parse_line(Token_Line* line, enum _E_Parse_Type type)
{
	if (line->length != 4)
	{
		fprintf(stderr, "malformed line in obj file\n");
		exit(1);
	}

	void* out = NULL;
	switch (type) {
	case _UINT32_TRIPLE:
		if ((out = malloc(sizeof(uint32_t) * 3)) == NULL)
		{
			fprintf(stderr, "malloc failed in obj importer\n");
			exit(1);
		}
		for (size_t i = 1; i < 4; i++)
		{
			uint32_t parsed = (uint32_t) strtoul(line->tokens[i], NULL, 10);
			uint32_t* write_to = (uint32_t*) (out + (sizeof(uint32_t) * (i - 1)));
			*write_to = parsed;
		}
		break;
	case _VECTOR:
		if ((out = malloc(sizeof(Vector))) == NULL)
		{
			fprintf(stderr, "malloc failed in obj importer\n");
			exit(1);
		}

		{
			double parsed[3];
			for (size_t i = 1; i < 4; i++)
				parsed[i - 1] = strtod(line->tokens[i], NULL);
			Vector tmp = {parsed[0], parsed[1], parsed[2]};
			out = &tmp;
		}
		break;
	}

	return out;
}

static void* _parse_all(Token_Line** line, size_t end_ptr, enum _E_Parse_Type type)
{
	switch (type) {
	case _UINT32_TRIPLE:
		{
			uint32_t* out;
			if ((out = malloc(sizeof(uint32_t) * 3 * end_ptr)) == NULL)
			{
				fprintf(stderr, "malloc failed in obj importer\n");
				exit(1);
			}
			for (size_t i = 0; i < end_ptr; i++)
			{
				uint32_t* tmp = _parse_line(line[i], type);
				out[i * 3] = tmp[0];
				out[i * 3 + 1] = tmp[1];
				out[i * 3 + 2] = tmp[2];
			}
			return out;
		}
		break;
	case _VECTOR:
		{
			Vector* out;
			if ((out = malloc(sizeof(Vector*) * end_ptr)) == NULL)
			{
				fprintf(stderr, "malloc failed in obj importer\n");
				exit(1);
			}
			for (size_t i = 0; i < end_ptr; i++)
				out[i] = *(Vector*) _parse_line(line[i], type);
			return out;
		}
		break;
	default:
		break;
	}

	return NULL;
}

#ifdef UNIT_TEST 
static void _debug_print(Token_Line** token_line_array)
{
	Token_Line* tmp;
	size_t ctr = 0;
	while ((tmp = token_line_array[ctr++]) != NULL)
	{
		for (size_t j = 0; j < tmp->length; j++)
			printf("<%s> ", tmp->tokens[j]);
		printf("\n");

		if (ctr == 5)
		{
			printf("...\n");
			break;
		}
	}
}

static void _test_parse(Token_Line** vertex_data, Token_Line** normal_data, 
						Token_Line** face_data, size_t v_ctr, size_t n_ctr, size_t f_ctr)
{
	printf("\nRead:\n");
	printf("\nVertex Data\n");
	_debug_print(vertex_data);

	printf("\nNormal Data\n");
	_debug_print(normal_data);

	printf("\nFace Data\n");
	_debug_print(face_data);

	printf("\nParse:\n");
	printf("\nVector (vertex data line 1)\n");
	Vector* vec = _parse_line(vertex_data[0], _VECTOR);
	printf("%f %f %f\n", vec->x, vec->y, vec->z);

	printf("\nInts (face data line 1)\n");
	uint32_t* nums = _parse_line(face_data[0], _UINT32_TRIPLE);
	printf("%u %u %u\n", nums[0], nums[1], nums[2]);

	printf("\nAll vertex data\n");
	Vector* all_vectors = _parse_all(vertex_data, v_ctr, _VECTOR);
	for (size_t i = 0; i < v_ctr; i++) 
	{
		if (i == 5) 
		{
			printf("...\n");
			break;
		}
		printf("%f %f %f\n", all_vectors[i].x, all_vectors[i].y, all_vectors[i].z);
	}

	printf("\nAll face data\n");
	uint32_t* all_faces = _parse_all(face_data, f_ctr, _UINT32_TRIPLE);
	for (size_t i = 0; i < f_ctr; i ++)
	{
		if (i == 5) 
		{
			printf("...\n");
			break;
		}
		printf("%u %u %u\n", all_faces[i * 3], all_faces[i * 3 + 1], all_faces[i * 3  + 2]);
	}
}
#endif

/*
 * PUBLIC:
 */

Obj_Object* parse_obj_file(char* file_name)
{
	Obj_Object* out;
	if ((out = malloc(sizeof(Obj_Object))) == NULL)
	{
		fprintf(stderr, "malloc failed in obj importer\n");
		exit(1);
	}

	out->length = 0;
	for (size_t i = 0; i < 1000; i++)
	{
		out->tris[i] = malloc(sizeof(Hittable*));
		if (out->tris[i] == NULL)
		{
			fprintf(stderr, "malloc failed in obj importer\n");
			exit(1);
		}
	}

	FILE* file = fopen(file_name, "r");

	size_t v_ctr = 0;
	size_t n_ctr = 0;
	size_t f_ctr = 0;

	Token_Line** vertex_data = _get_lines_with_prefix(file, &v_ctr, "v");
	Token_Line** normal_data = _get_lines_with_prefix(file, &n_ctr, "vn");
	Token_Line** face_data = _get_lines_with_prefix(file, &f_ctr, "f");

	fclose(file);

#ifdef UNIT_TEST
	_test_parse(vertex_data, normal_data, face_data, v_ctr, n_ctr, f_ctr);
#endif

	Vector* vertices = _parse_all(vertex_data, v_ctr, _VECTOR);
	Vector* normals  = _parse_all(normal_data, n_ctr, _VECTOR);
	uint32_t* faces  = _parse_all(face_data, f_ctr, _UINT32_TRIPLE);

	free(vertex_data);
	free(normal_data);
	free(face_data);

	return out;
}
