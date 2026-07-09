for i in range(65):
    print(f'#define HAMU_FOR_EACH_{i}(i, ...) HAMU_JSON_FIELD_ACTION(i) HAMU_FOR_EACH_{i-1}(__VA_ARGS__)')