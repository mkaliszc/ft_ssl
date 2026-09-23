#pragma once

# include "libft.h"
# include <stdint.h>
# include <stddef.h>
# include <errno.h>
# include <string.h>

// Used for comparing bitmask information
# define FLAG_P		(1 << 0)
# define FLAG_Q		(1 << 1)
# define FLAG_R		(1 << 2)

// Used to be more clearer, not big signification
# define IS_FLAG	1
# define IS_FLAG_S	2
# define IS_TOK		3

# define COMMAND_INFO "[INFO] Avaible command : md5, sha256\n"

# define READ_SIZE			4096	// size of one read() and of one stdin chunk
# define DIGEST_MAX_SIZE	64		// largest digest in bytes (whirlpool ready)
# define BLOCK_SIZE			64		// md5 / sha256 block size in bytes
# define LEN_OFFSET			56		// where the 64-bit length field starts in the last block

typedef enum e_src
{
	SRC_STDIN,
	SRC_FILE,
	SRC_STRING
}	t_src;

typedef struct s_command_ssl {
	char	*func_name;
	int		(*run)(int argc, char **argv);	
}	t_command_ssl;

// One piece of stdin kept for -p
typedef struct s_chunk {
	uint8_t			data[READ_SIZE];
	size_t			len;
	struct s_chunk	*next;
}	t_chunk;

typedef struct s_operand {
	char				*input_name; // File name or String
	t_chunk				*chunks; // stdin content, only filled when -p is set
	t_src				src_type;
	struct s_operand	*next;
}	t_operand;

typedef struct s_digest_ctx {
	union {
		uint32_t	md5[4];
		uint32_t	sha256[8];
		uint64_t	whirlpool[8];
	}				state;
	uint8_t			buffer[BLOCK_SIZE];
	size_t			buf_len;
	uint64_t		total_len;
}	t_digest_ctx;

// Shared by every 32-bit word / 64-byte block algorithm (md5, sha256)
typedef void	(*t_transform)(uint32_t *state, const uint8_t *block);
typedef void	(*t_store64)(uint8_t *dst, uint64_t value);

typedef struct s_digest_spec {
	const char	*label;
	size_t		digest_size;
	void		(*init)(t_digest_ctx *ctx);
	void		(*update)(t_digest_ctx *ctx, const uint8_t *in, size_t len);
	void		(*final)(t_digest_ctx *ctx, uint8_t *out);
}	t_digest_spec;

typedef struct s_options {
	uint8_t					flags;
	t_operand				*operands;
	const t_digest_spec		*spec;
}	t_options;

// Core : command dispatch
void		dispatch_cmd(int argc, char **argv);

// Digest family : one entry point per command (referenced by command_list)
int			md5_func(int argc, char **argv);
int			sha256_func(int argc, char **argv);

// Digest family : pipeline  parse -> exec -> display -> clean
bool		digest_parser(int argc, char **argv, t_options *opt);
bool		handle_operand(char *name, t_src src, t_operand **operands);
int			digest_exec(t_options *opt);
void		print_result(t_options *opt, t_operand *op, const uint8_t *digest);
void		ssl_digest_cleaner(t_options *opt);

// Digest family : stdin kept for -p
bool		add_chunk(t_chunk **lst, const uint8_t *data, size_t len);
void		free_chunks(t_chunk **lst);

// Digest core
void		digest_update(t_digest_ctx *ctx, uint32_t *state, t_transform tf, const uint8_t *in, size_t len);
void		digest_pad(t_digest_ctx *ctx, uint32_t *state, t_transform tf, t_store64 store_len);
void		md5_transform(uint32_t state[4], const uint8_t block[64]);
void		sha256_transform(uint32_t state[8], const uint8_t block[64]);

// Common byte helpers
void		digest_to_hex(const uint8_t *digest, size_t len, char *dst);
uint32_t	load32_le(const uint8_t *src);
uint32_t	load32_be(const uint8_t *src);
void		store32_le(uint8_t *dst, uint32_t value);
void		store32_be(uint8_t *dst, uint32_t value);
void		store64_le(uint8_t *dst, uint64_t value);
void		store64_be(uint8_t *dst, uint64_t value);

// Debug
void		print_op_lst(t_operand *lst);
