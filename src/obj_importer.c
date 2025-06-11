#include "obj_importer.h"
#include <string.h>

/*
 * PRIVATE:
 */

static const uint8_t max_token_chars = 5;
static const uint8_t max_line_tokens = 10;

static char** allocate_tokens()
{
	char** tokens;
	if ((tokens = malloc(max_line_tokens * sizeof(char*))) != NULL)
	{
		for (uint8_t i = 0; i < max_line_tokens; i++)
		{
			tokens[i] = malloc(max_token_chars * sizeof(char) + 1);
			if (tokens[i] == NULL)
			{
				fprintf(stderr, "malloc failed in obj importer");
				exit(1);
			}
		}
	} else 
	{
		fprintf(stderr, "malloc failed in obj importer");
		exit(1);
	}
	return tokens;
}

static Token_Array* tokenize_string(char* str, char delims[], uint8_t delim_len)
{
	Token_Array* out = malloc(sizeof(Token_Array));
	out->tokens = allocate_tokens();
	out->len = 0;

	char* buf;
	if ((buf = malloc(max_token_chars * sizeof(char) + 1)) != NULL)
		buf[0] = '\0'; // null terminator
	else 
	{
		fprintf(stderr, "malloc failed in obj importer");
		exit(1);
	}

	char c;
	int8_t ctr = -1;
	while ((c = str[++ctr]) != '\0') // loop over whole input string
	{
		bool is_delim = false;
		for (uint8_t delim_idx = 0; delim_idx < delim_len; delim_idx++)
		{
			if (c == delims[delim_idx])
			{
				is_delim = true;
				break;
			}
		}
		if (is_delim) 
		{
			// flush and clear buffer
			memcpy(*(out->tokens + out->len++), buf, max_token_chars);
			memset(buf, 0, max_token_chars * sizeof(char) + 1);
			buf[0] = '\0';
		} else 
		{
			// add current character to buffer
			int8_t buf_head = -1;
			while (buf[++buf_head] != '\0');
			if (buf_head >= max_token_chars)
			{
				fprintf(stderr, "buffer overrun in tokenize\n");
				exit(1);
			}
			buf[buf_head] = c;
			buf[buf_head + 1] = '\0';
		}
	}

	free(buf);
	return out;
}

static Token_Array** read_after_prefix(FILE* file, char* prefix)
{
	const uint8_t read_buf_len = 30;

	Token_Array** toks;
	uint16_t toks_head = 0;
	if ((toks = malloc(100 * sizeof(Token_Array*))) == NULL)
	{
		fprintf(stderr, "malloc failed in obj importer\n");
		exit(1);
	}

	char delimiters[] = {'\n', '\r', ' '};
	char read_buf[read_buf_len];
	while (fgets(read_buf, read_buf_len, file))
	{
		Token_Array* token_arr = tokenize_string(read_buf, delimiters, 3);
		if (token_arr->len > 0)
		{
			if (strcmp(token_arr->tokens[0], prefix) == 0) // matches
				toks[toks_head++] = token_arr;
		}
	}
	return toks;
}

/*
 * PUBLIC:
 */

Obj_Object* parse_obj(char* file_name)
{
	Obj_Object* out;
	if ((out = malloc(sizeof(Obj_Object))) != NULL)
	{
		out->len = 0;
		for (uint16_t i = 0; i < 1000; i++)
		{
			out->tris[i] = malloc(sizeof(Hittable*));
			if (out->tris[i] == NULL)
			{
				fprintf(stderr, "malloc failed in obj importer\n");
				exit(1);
			}
		}
	} else 
	{
		fprintf(stderr, "malloc failed in obj importer\n");
		exit(1);
	}

	FILE* file = fopen(file_name, "r");

	Token_Array** vertex_data = read_after_prefix(file, "v");
	Token_Array** normal_data = read_after_prefix(file, "vn");
	Token_Array** face_data = read_after_prefix(file, "f"); // must all be triangles

	fclose(file);

	free(vertex_data);
	free(normal_data);
	free(face_data);

	return out;
}
