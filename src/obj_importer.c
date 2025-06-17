#include "obj_importer.h"
#include "hittable.h"

/*
 * PRIVATE:
 */

/*
 * Used internally to differentiate between the 2 types of data to be read from 
 * the obj file: coordinates (vectors) or indices (uint triples)
 */
static enum _E_Parse_Type {
	_UINT32_TRIPLE,
	_VECTOR
} _E_Parse_Type;

static struct Token_Line {
	char** tokens;
	size_t length;
} Token_Line;


static const size_t _max_token_chars = 20; // maximum amount of characters per word
static const size_t _max_line_tokens = 4; // maximum amount of words per line

/*
 * Allocates a char** (array of strings) to be used to read words from the input 
 * file. Allocated heap memory is _max_line_tokens * (_max_token_chars * sizeof(char) + 1).
 * The allocated string array leaves an extra character in each 'token' for the 
 * null terminator character '\0'.
 */
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

/*
 * Takes in the input string (taken from the file) and separates it into an 
 * array of 'tokens' which are returned as a pointer to token_line. This token_line 
 * is allocated using the _alloc_str_arr method above. The amount of delimiters 
 * must also be passed in. The input string is assumed to be null terminated as 
 * this is the default for lines read from a file input stream.
 *
 * This method works by allocating a small buffer which is large enough to 
 * store one word or 'token' and scanning it across the input. When a delimiter 
 * is reached (usually a space or a null terminator) the buffer is flushed into 
 * the output token_line struct. This buffer is then reused for the next word.
 * If the buffer cannot be allocated, the application exits with code 1. The buffer 
 * is null terminated so its length can be measured, if the length of the buffer 
 * exceeds its maximum length, then the application exits with code 1.
 *
 * This method outputs the array of strings as a pointer to a token_line 
 * struct which stores each string as well as the number of tokens in that line.
 * If this struct cannot be allocated, the application exits with code 1.
 */
static struct Token_Line* _tokenize(char* str, char delims[], size_t delim_cnt)
{
	struct Token_Line* line;
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

	for (size_t ctr = 0; ; ctr++)
	{
		char c = str[ctr];
		bool is_delim = false;
		for (size_t i = 0; (i < delim_cnt) && !is_delim; i++)
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

		if (str[ctr] == '\0') // null terminated input string
			break;
	}

	free(buf);
	return line;
}

/*
 * Reads in every line from the input file, tokenizes each one and saves the 
 * lines that start with (prefix) to the output array of token_line pointers.
 * The amount of lines added to the output is stored in (end_ptr) which should 
 * be initialized to 0 before passing it into the method.
 *
 * There is a hard-coded limit of 650 lines that this method can handle as this 
 * is how many I needed for my demo, in the future i could do this in chunks and 
 * remove this restriction but it is ok for now.
 *
 * The read head of the input stream (file) is reset inside this method so that 
 * it can be used multiple times to find different prefixes.
 *
 * When tokenizing each line in the file, it is split at new lines, carriage returns,
 * and spaces. The lines read from the file are all null-terminated and this is 
 * handled in the _tokenize method.
 */
static struct Token_Line** _get_lines_with_prefix(FILE* file, size_t* end_ptr, char* prefix)
{
	struct Token_Line** toks;
	size_t toks_head = 0;
	if ((toks = malloc(650 * sizeof(struct Token_Line*))) == NULL)
	{
		fprintf(stderr, "malloc failed in obj importer\n");
		exit(1);
	}

	char delims[] = {'\n', '\r', ' '};
	char read_buf[45];
	fseek(file, 0, SEEK_SET);
	while (fgets(read_buf, sizeof(read_buf) / sizeof(read_buf[0]), file))
	{
		struct Token_Line* token_arr = _tokenize(read_buf, delims, 3);
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

/*
 * Accepts a tokenized line and converts it into a given type of data. OBJ files 
 * contain two main types of data (floating point coordinate components: vectors, 
 * and indices into these coordinates: uint triples). The method will output 
 * either a vector or an array of uint32_t depending on the parse type passed.
 *
 * Vector lines from the file are given in the format <prefix> <x> <y> <z> and 
 * can are already split into pieces when tokenizing the whole string so they 
 * can just be converted using the built in strtod function. When outputting a 
 * vector, it must be allocated on the heap. If this allocation fails, the 
 * application exits with code 1.
 *
 * Index lines from the file are given in the format <prefix> <ca/ta/na> <cb/tb/nb> <cc/tc/nc>
 * where c means a vector index, t means a texture index, and n means a normal index.
 * These indices refer to the vector lines in the file and are all 1-indexed.
 * My path tracer does not yet support textures so I ignore the middle component of each 
 * index triple and only look at the first (vertex index) and last (normal index) of each.
 * To parse this, I further tokenize each token by splitting at the '/' character 
 * and then output the indices as they are (still 1-indexed) as an array of 6 uint32_t.
 * These are in the format: vert idx a, vert idx b, vert idx c, norm idx a, norm idx b, norm idx c.
 */
static void* _parse_line(struct Token_Line* line, enum _E_Parse_Type type)
{
	if (line->length != 4)
	{
		fprintf(stderr, "malformed line in obj file\n");
		exit(1);
	}

	void* out = NULL;
	switch (type) {
	case _UINT32_TRIPLE:
		if ((out = malloc(sizeof(uint32_t) * 6)) == NULL)
		{
			fprintf(stderr, "malloc failed in obj importer\n");
			exit(1);
		}
		for (size_t i = 1; i < 4; i++)
		{
			char delims[] = {'/', '\n', '\r', '\0'};
			struct Token_Line* sub_tokens = _tokenize(line->tokens[i], delims, 4);

			uint32_t parsed_1 = (uint32_t) strtoul(sub_tokens->tokens[0], NULL, 10);
			uint32_t parsed_2 = (uint32_t) strtoul(sub_tokens->tokens[2], NULL, 10);

			uint32_t* write_to = (uint32_t*) (out + (sizeof(uint32_t) * (i - 1)));
			*write_to = parsed_1; // face data

			write_to = (uint32_t*) (out + (sizeof(uint32_t) * 3) + (sizeof(uint32_t) * (i - 1)));
			*write_to = parsed_2; // face data
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
			for (size_t i = 0; i < 3; i++)
				parsed[i] = strtod(line->tokens[i + 1], NULL);

			Vector tmp = {parsed[0], parsed[1], parsed[2]};
			out = &tmp;
		}
		break;
	}

	return out;
}

/*
 * Iteratively parses each token_line in lines, treating them as if they are of 
 * data type (type) and outputs them as an array.
 *
 * If the type is _VECTOR then the output is an array of vectors of size (count).
 * This requires allocating an array of Vector structs. If this malloc fails, 
 * the application exits with code 1.
 *
 * If the type is _UINT32_TRIPLE then the output is an array of components of 
 * size (count * 6). To see why, see _parse_line. This requires allocating an 
 * array of integers, if this allocation fails, then the application exits with code 1.
 */
static void* _parse_all(struct Token_Line** lines, size_t count, enum _E_Parse_Type type)
{
	switch (type) {
	case _UINT32_TRIPLE:
		{
			uint32_t* out;
			if ((out = malloc(sizeof(uint32_t) * 6 * count)) == NULL)
			{
				fprintf(stderr, "malloc failed in obj importer\n");
				exit(1);
			}
			for (size_t i = 0; i < count; i++)
			{
				uint32_t* tmp = _parse_line(lines[i], type);

				out[i * 6] = tmp[0];
				out[i * 6 + 1] = tmp[1];
				out[i * 6 + 2] = tmp[2];
				out[i * 6 + 3] = tmp[3];
				out[i * 6 + 4] = tmp[4];
				out[i * 6 + 5] = tmp[5];

				free(tmp);
			}
			return out;
		}
		break;
	case _VECTOR:
		{
			Vector* out;
			if ((out = malloc(sizeof(Vector) * count)) == NULL)
			{
				fprintf(stderr, "malloc failed in obj importer\n");
				exit(1);
			}
			for (size_t i = 0; i < count; i++)
			{
				Vector* tmp = _parse_line(lines[i], type);
				out[i] = *tmp;
			}
			return out;
		}
		break;
	default:
		break;
	}

	return NULL;
}

#ifdef UNIT_TEST 
static void _debug_print(struct Token_Line** token_line_array)
{
	struct Token_Line* tmp;
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

static void _test_parse(struct Token_Line** vertex_data, struct Token_Line** normal_data, 
						struct Token_Line** face_data, size_t v_ctr, size_t n_ctr, size_t f_ctr)
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
	printf("%u %u %u %u %u %u\n", nums[0], nums[1], nums[2], nums[3], nums[4], nums[5]);

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

	printf("\nAll normal data\n");
	Vector* all_normals = _parse_all(normal_data, n_ctr, _VECTOR);
	for (size_t i = 0; i < n_ctr; i++) 
	{
		if (i == 5) 
		{
			printf("...\n");
			break;
		}
		printf("%f %f %f\n", all_normals[i].x, all_normals[i].y, all_normals[i].z);
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
		printf("%u %u %u %u %u %u\n", all_faces[i * 6], all_faces[i * 6 + 1], 
			   all_faces[i * 6 + 2], all_faces[i * 6 + 3], all_faces[i * 6 + 4],
			   all_faces[i * 6 + 5]);
	}
}
#endif

/*
 * PUBLIC:
 */

/*
 * Reads a wavefront OBJ file and extracts the relevant vertex coordinates, 
 * normal coordinates, and face data to reconstruct the mesh it represents.
 * This new mesh is offsetted by a vector {x, y, z} and is assigned the given 
 * material. The mesh is returned as a pointer to an Obj_Object which stores 
 * a pointer to each triangle of the mesh and the amount that there are. 
 * This method must allocate an Obj_Object struct as well as a hittable 
 * struct for each triangle in the mesh, if these allocations fail then the 
 * application exits with code 1.
 *
 * It is required that the mesh is made of ONLY TRIANGLES.
 */
Obj_Object* parse_obj_file(char* file_name, double x, double y, double z, Material material)
{
	// reading in the file
	FILE* file = fopen(file_name, "r");

	size_t v_ctr = 0;
	size_t n_ctr = 0;
	size_t f_ctr = 0;

	struct Token_Line** vertex_data = _get_lines_with_prefix(file, &v_ctr, "v");
	struct Token_Line** normal_data = _get_lines_with_prefix(file, &n_ctr, "vn");
	struct Token_Line** face_data = _get_lines_with_prefix(file, &f_ctr, "f");

	fclose(file);

#ifdef UNIT_TEST
	_test_parse(vertex_data, normal_data, face_data, v_ctr, n_ctr, f_ctr);
#endif

	// parsing out the values
	Vector* vertex_idxs = _parse_all(vertex_data, v_ctr, _VECTOR);
	Vector* normal_idxs  = _parse_all(normal_data, n_ctr, _VECTOR);
	uint32_t* faces = _parse_all(face_data, f_ctr, _UINT32_TRIPLE);

	// constructing the obj object
	Obj_Object* out;
	if ((out = malloc(sizeof(Obj_Object))) == NULL)
	{
		fprintf(stderr, "malloc failed in obj importer\n");
		exit(1);
	}

	Vector pos_offset = {x, y, z};
	out->length = f_ctr;
	for (size_t i = 0; i < out->length; i++)
	{
		Vector a = vertex_idxs[faces[i * 6] - 1];
		Vector b = vertex_idxs[faces[i * 6 + 1] - 1];
		Vector c = vertex_idxs[faces[i * 6 + 2] - 1];
		Vector na = normal_idxs[faces[i * 6 + 3] - 1];
		Vector nb = normal_idxs[faces[i * 6 + 4] - 1];
		Vector nc = normal_idxs[faces[i * 6 + 5] - 1];

		a = vec_add(a, pos_offset);
		b = vec_add(b, pos_offset);
		c = vec_add(c, pos_offset);

		out->tris[i] = hittable_new_tri(a, b, c, na, nb, nc, material);
	}

	// cleaning up
	free(vertex_data);
	free(normal_data);
	free(face_data);
	free(vertex_idxs);
	free(normal_idxs);
	free(faces);

	return out;
}
