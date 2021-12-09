#pragma once

namespace log {
int				geterrors();
void			error(const char* position, const char* format, ...);
void			errorv(const char* position, const char* format);
void            setfile(const char* v);
void            seturl(const char* v);
}
