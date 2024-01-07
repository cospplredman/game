
//variable number of arguments library

//for arguments that all have the same type
#define ARG_SZ(T, ...) (sizeof((T[]){__VA_ARGS__})/sizeof(T))
int max(size_t sz, int s, ...){
    va_list v;
    va_start(v, s);

    int mx = s;
    for(int i = 0; i < sz-1; i++){
        int j = va_arg(v, int);
        if(j > mx)
            mx = j;
    }

    va_end(v);
    return mx;
}

#define max(...) max(ARG_SZ(int, __VA_ARGS__), __VA_ARGS__)

