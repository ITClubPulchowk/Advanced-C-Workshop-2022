#define lambda(return_type, function_body) \
  ({ \
    return_type anon_func_name_ function_body \
    anon_func_name_; \
  })

int main() {
int (*max)(int, int) = lambda (int, (int x, int y) { return x > y ? x : y; });
}