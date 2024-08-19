#define N 100

void node_0(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_a
    float cache_in[N];
    float cache_out[N];

    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }

    for(int i = 0; i < iters; i++) {
        float x = cache_in[i % N];
        float y = x + i;
        cache_out[i % N] = y;
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_1(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_a
    float cache_in[N];
    float cache_out[N];

    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }

    for(int i = 0; i < iters; i++) {
        float x = cache_in[i % N];
        float y = x + i;
        cache_out[i % N] = y;
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_2(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_a
    float cache_in[N];
    float cache_out[N];

    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }

    for(int i = 0; i < iters; i++) {
        float x = cache_in[i % N];
        float y = x + i;
        cache_out[i % N] = y;
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_3(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_a
    float cache_in[N];
    float cache_out[N];

    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }

    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < iters; j++) {
            float x = cache_in[j % N];
            float y = x + j;
            cache_out[j % N] = y; // just to make sure both inner nodes have the same latency
        }
        for(int j = 0; j < iters; j++) {
            float x = cache_in[j % N]; // just to make sure both inner nodes have the same latency
            float z = j - x;
	    cache_out[j % N] = z;
        }
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_3_1_a_1(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_b
    float cache_in[N];
    float cache_out[N];
    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }
    for(int j = 0; j < iters/2; j++) {
	float x = cache_in[j % N];
	float y = x + j;
	cache_out[j % N] = y; // just to make sure both inner nodes have the same latency
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_3_1_b_1(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_b
    float cache_in[N];
    float cache_out[N];
    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }
    for(int j = 0; j < iters/2; j++) {
        float x = cache_in[j % N]; // just to make sure both inner nodes have the same latency
        float z = j - x;
        cache_out[j % N] = z;
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_3_1_a_2(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_b
    float cache_in[N];
    float cache_out[N];
    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }
    for(int j = iters/2; j < iters; j++) {
	float x = cache_in[j % N];
	float y = x + j;
	cache_out[j % N] = y; // just to make sure both inner nodes have the same latency
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_3_1_b_2(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_b
    float cache_in[N];
    float cache_out[N];
    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }
    for(int j = iters/2; j < iters; j++) {
        float x = cache_in[j % N]; // just to make sure both inner nodes have the same latency
        float z = j - x;
        cache_out[j % N] = z;
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}



void node_3_2_a_1(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_b
    float cache_in[N];
    float cache_out[N];
    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }
    for(int j = 0; j < iters/2; j++) {
	float x = cache_in[j % N];
	float y = x + j;
	cache_out[j % N] = y; // just to make sure both inner nodes have the same latency
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_3_2_b_1(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_b
    float cache_in[N];
    float cache_out[N];
    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }
    for(int j = 0; j < iters/2; j++) {
        float x = cache_in[j % N]; // just to make sure both inner nodes have the same latency
        float z = j - x;
        cache_out[j % N] = z;
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_3_2_a_2(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_b
    float cache_in[N];
    float cache_out[N];
    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }
    for(int j = iters/2; j < iters; j++) {
	float x = cache_in[j % N];
	float y = x + j;
	cache_out[j % N] = y; // just to make sure both inner nodes have the same latency
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_3_2_b_2(float * in, float * out, int iters) {
#pragma hls interface m_axi port=in bundle=bundle_a
#pragma hls interface m_axi port=out bundle=bundle_b
    float cache_in[N];
    float cache_out[N];
    for(int i = 0; i < N; i++) {
        cache_in[i] = in[i];
    }
    for(int j = iters/2; j < iters; j++) {
        float x = cache_in[j % N]; // just to make sure both inner nodes have the same latency
        float z = j - x;
        cache_out[j % N] = z;
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}

void node_4(float * in_0, float * in_1, float * in_2, float * out, int iters) {
#pragma hls interface m_axi port=in_0 bundle=bundle_a
#pragma hls interface m_axi port=in_1 bundle=bundle_b
#pragma hls interface m_axi port=in_2 bundle=bundle_c
#pragma hls interface m_axi port=out bundle=bundle_a
    float cache_in_0[N];
    float cache_in_1[N];
    float cache_in_2[N];
    float cache_out[N];

    for(int i = 0; i < N; i++) {
        cache_in_0[i] = in_0[i];
    }
    for(int i = 0; i < N; i++) {
        cache_in_1[i] = in_1[i];
    }

    for(int i = 0; i < N; i++) {
        cache_in_2[i] = in_2[i];
    }

    for(int i = 0; i < iters; i++) {
        float x = cache_in_0[i % N];
        float y = x + i;
        cache_out[i % N] = y;
    }
    for(int i = 0; i < N; i++) {
        out[i] = cache_out[i];
    }
}
