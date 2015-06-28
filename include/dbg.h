/*
** dbg.h for irc in /home/xxx/Epitech/2015/my_irc
** 
** Made by 
** Login   <xxx@epitech.eu>
** 
** Started on  Tue Apr 14 15:34:29 2015 
** Last update Tue Apr 14 15:43:40 2015 
*/

#ifndef		__dbg_h__
# define	__dbg_h__

# include	<stdio.h>
# include	<errno.h>
# include	<string.h>

# ifdef NDEBUG
#  define debug(M, ...)
#  else
#  define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
# endif

# define clean_errno() (errno == 0 ? "None" : strerror(errno))

# define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

# define log_args(M, ...) fprintf(stderr, M "\n", ##__VA_ARGS__)

# define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

# define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

# define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

# define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

# define check_mem(A) check((A), "Out of memory.")

# define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

# define check_args(A, M, ...) if(!(A)) { log_args(M, ##__VA_ARGS__); errno=0; goto error; }
#endif
