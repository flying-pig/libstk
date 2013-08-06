/*
 *  Copyright (c) 1996 - 2013 SINA Corporation, All Rights Reserved.
 *
 *  stk_string.c:  Huaying <huaying@staff.sina.com.cn>
 *  
 *  simple tool kit: string utils.
 */
#include <stdlib.h>
#include <string.h>

#include "stk_string.h"

char *strtrim_right(char *p, char c)
{
  char *end;
  int len;
  len = strlen(p);
  while (*p && len) {
    end = p + len - 1;
    if (c == *end) *end = '\0';
    else break;
    len = strlen(p);
  }

  return p;
}

char *strtrim_left(char *p, char c)
{
  char *header = p;
  
  while (*header) {
    if (*header == c) ++header;
    else break;
  }

  return header;
}

char *strtrim(char *p, char c)
{
  char *str;
  str = strtrim_left(p, c);
  strtrim_right(str, c);
}

char *trim_right_m(char *p)
{
  while (*p && isspace(*p)) p++;
  return p; 
}

char *trim_left_m(char *p)
{
  char *end;
  end = p + strlen(p) - 1;
  while (end > p && isspace(*end)) --end;
  *(end + 1) = '\0';
  return p;
}

char *trim_right_r(char *str)
{
	int len = 0;
	char *end_str;

	if (str == NULL)
		return NULL;
	len = strlen(str);
	if (len == 0)
		return NULL;

	end_str = str + len - 1;
	while (isspace(*end_str) && end_str != str) {
		end_str--;
	}

	*(end_str + 1) = '\0';

	return str;
}
char *trim_left_r(char *str)
{
	int len = 0;
	char *str_bak;
	char *str_tmp;

	if (str == NULL)
		return NULL;
	len = strlen(str);
	if (len == 0)
		return NULL;

	str_bak = str;
	while (isspace(*str_bak) && (*str_bak) != '\0') {
		str_bak++;
	}

	str_tmp = str;
	if (str_bak != str_tmp) {
		while (*str_bak != '\0') {
			*(str_tmp++) = *(str_bak++);
		}
		*str_tmp = '\0';
	}

	return str;
}

char *trim_m(char *p)
{
  char *str;
  str = trim_right_m(p);
  return trim_left_m(str);
}

char *trim_r(char *p)
{
  char *str;
  str = trim_right_r(p);
  return trim_left_r(str);
}

/* len: out buffer len */
size_t trim_left(char *out, size_t len, const char *p)
{
  size_t out_size;
  const char *end;
  if (len == 0) return 0;

  while (*p && isspace(*p)) ++p;
  if (*p == '\0') {
    *out = '\0';
    return 0;
  }

  end = p + strlen(p) - 1;
  out_size = strlen(p); 
  out_size = (end - p) < len - 1 ? (end - p) : len - 1;
  memcpy(out, p, out_size);
  out[out_size] = '\0';

  return out_size;
}

size_t trim_right(char *out, size_t len, const char *p)
{
  size_t out_size;
  const char *end;
  if (len == 0) return 0;

  end = p + strlen(p) - 1;
  while (end > p && isspace(*end)) --end;
  end++;

  out_size = (end - p) < len - 1 ? (end - p) : len - 1;
  memcpy(out, p , out_size);
  out[out_size] = '\0';

  return out_size;
}

size_t trim(char *out, size_t len, const char *p)
{
  if (len == 0) return 0;

  const char *end;
  size_t out_size;

  while(*p && isspace(*p)) ++p;
  if(*p == '\0') {
    *out = '\0';
    return 0;
  }

  end = p + strlen(p) - 1;
  while (end > p && isspace(*end)) --end;
  end++;

  out_size = (end - p) < len - 1 ? (end - p) : len - 1;
  memcpy(out, p , out_size);
  out[out_size] = '\0';

  return out_size;
}

