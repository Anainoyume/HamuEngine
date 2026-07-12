struct float4
{
    float x, y, z, w;
};

float dot(const float4& a, const float4& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

constexpr int N = 1 << 10;

float4 vec_a[N];
float4 vec_b[N];
float ans[N];

void init() {
    for (int i = 0; i < N; i++) {
        vec_a[i] = float4{float(i) - 2.0f, float(i) + 3.0f, float(i) - 27.0f, float(i) - 113.0f};
        vec_b[i] = float4{float(i) - 3.0f * i, float(i) + 114.0f, float(i) - 1.0f, float(i) + i};
    }
}

int main() {

    init();

    for (int i = 0; i < N; i++) {
        ans[i] = dot(vec_a[i], vec_b[i]);
    }

}