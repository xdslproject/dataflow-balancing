
    #include <stdio.h>
    #include <stdlib.h>
    #include <CL/cl.h>
    #include "omp.h"
                      
    #define N 100     
    #define K 100000
                      
    int main(int argc, char * argv[]) {      
	if(argc < 3) {
		printf("host_cached_unbalanced <pipelined> <n_iters>\n");
		printf("<pipelined>: 0 - no; 1 - yes\n");

		return -1;
	}

	int pipelined = atoi(argv[1]);
	int n_iters = atoi(argv[2]);
	
        cl_device_id device_id;
        int err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ACCELERATOR, 1, &device_id, NULL);
        cl_context context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
        if (!context) 
        {             
            printf("Error: Failed to create a compute context!\n");
            return EXIT_FAILURE;
        }             
                      
        // Create a command commands
        //            
        cl_command_queue commands = clCreateCommandQueue(context, device_id, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
        if (!commands)
        {             
            printf("Error: Failed to create a command commands!\n");
            return EXIT_FAILURE;
        }             
                      
                      
                      
        FILE * f;     
        f = fopen("linked_dram-all_nodes.xclbin", "r");
                      
        fseek(f, 0, SEEK_END);
        size_t file_size = ftell(f);
        fseek(f, 0, SEEK_SET);
                      
        const unsigned char * binary = malloc(file_size * sizeof(const unsigned char));
        fread(binary, file_size, 1, f);
                      
                      
        cl_int binary_status;
        cl_program program = clCreateProgramWithBinary(context, 1, &device_id, &file_size, &binary, &binary_status, &err);
        if (!program) {
            printf("Error: Failed to create compute program!\n");
        }             
                      
                      
        //// Build the program executable
        //            
        err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
        if (err != CL_SUCCESS)
        {             
            size_t len;
            char buffer[2048];
                      
            printf("Error: Failed to build program executable!\n");
            clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
            printf("%s\n", buffer);
            exit(1);  
        }             
        cl_int iters_node_0 = K*N;
	cl_int iters_node_1 = 2*K*N;
	cl_int iters_node_2 = 2.25*K*N;
	cl_int iters_node_3 = K*N/2;
	cl_int iters_node_4 = 2*K*N;
	
                      
        float * in = (float *)malloc(N * sizeof(float));
        float * out = (float *)malloc(N * sizeof(float));
        float * out_node_0 = (float*)malloc(N * sizeof(float));
	float * out_node_1 = (float*)malloc(N * sizeof(float));
	float * out_node_2 = (float*)malloc(N * sizeof(float));
	float * out_node_3_1_a_1 = (float*)malloc(N * sizeof(float));
	float * out_node_3_1_a_2 = (float*)malloc(N * sizeof(float));
	float * out_node_3_1_b_1 = (float*)malloc(N * sizeof(float));
	float * out_node_3_1_b_2 = (float*)malloc(N * sizeof(float));
	float * out_node_3_2_a_1 = (float*)malloc(N * sizeof(float));
	float * out_node_3_2_a_2 = (float*)malloc(N * sizeof(float));
	float * out_node_3_2_b_1 = (float*)malloc(N * sizeof(float));
	float * out_node_3_2_b_2 = (float*)malloc(N * sizeof(float));
	float * out_node_4 = (float*)malloc(N * sizeof(float));

    posix_memalign(&in,4096,100*sizeof(float)); 
    posix_memalign(&out,4096,100*sizeof(float)); 
    posix_memalign(&out_node_0,4096,100*sizeof(float)); 
    posix_memalign(&out_node_1,4096,100*sizeof(float)); 
    posix_memalign(&out_node_2,4096,100*sizeof(float)); 
    posix_memalign(&out_node_3_1_a_1,4096,100*sizeof(float)); 
    posix_memalign(&out_node_3_1_a_2,4096,100*sizeof(float)); 
    posix_memalign(&out_node_3_1_b_1,4096,100*sizeof(float)); 
    posix_memalign(&out_node_3_1_b_2,4096,100*sizeof(float)); 
    posix_memalign(&out_node_3_2_a_1,4096,100*sizeof(float)); 
    posix_memalign(&out_node_3_2_a_2,4096,100*sizeof(float)); 
    posix_memalign(&out_node_3_2_b_1,4096,100*sizeof(float)); 
    posix_memalign(&out_node_3_2_b_2,4096,100*sizeof(float)); 
    posix_memalign(&out_node_4,4096,100*sizeof(float)); 
	
                      
        for(int i = 0; i < N; i++) {
            in[i] = i+1;
            out[i] = 0;
        }             
        for(int i = 0; i < N; i++) out_node_0[i] = 0;
	for(int i = 0; i < N; i++) out_node_1[i] = 0;
	for(int i = 0; i < N; i++) out_node_2[i] = 0;
	for(int i = 0; i < N; i++) out_node_3_1_a_1[i] = 0;
	for(int i = 0; i < N; i++) out_node_3_1_a_2[i] = 0;
	for(int i = 0; i < N; i++) out_node_3_1_b_1[i] = 0;
	for(int i = 0; i < N; i++) out_node_3_1_b_2[i] = 0;
	for(int i = 0; i < N; i++) out_node_3_2_a_1[i] = 0;
	for(int i = 0; i < N; i++) out_node_3_2_a_2[i] = 0;
	for(int i = 0; i < N; i++) out_node_3_2_b_1[i] = 0;
	for(int i = 0; i < N; i++) out_node_3_2_b_2[i] = 0;
	for(int i = 0; i < N; i++) out_node_4[i] = 0;
	
                      
        cl_kernel node_0_kernel = clCreateKernel(program, "node_0", &err);
	cl_kernel node_1_kernel = clCreateKernel(program, "node_1", &err);
	cl_kernel node_2_kernel = clCreateKernel(program, "node_2", &err);
	cl_kernel node_3_1_a_1_kernel = clCreateKernel(program, "node_3_1_a_1", &err);
	cl_kernel node_3_1_a_2_kernel = clCreateKernel(program, "node_3_1_a_2", &err);
	cl_kernel node_3_1_b_1_kernel = clCreateKernel(program, "node_3_1_b_1", &err);
	cl_kernel node_3_1_b_2_kernel = clCreateKernel(program, "node_3_1_b_2", &err);
	cl_kernel node_3_2_a_1_kernel = clCreateKernel(program, "node_3_2_a_1", &err);
	cl_kernel node_3_2_a_2_kernel = clCreateKernel(program, "node_3_2_a_2", &err);
	cl_kernel node_3_2_b_1_kernel = clCreateKernel(program, "node_3_2_b_1", &err);
	cl_kernel node_3_2_b_2_kernel = clCreateKernel(program, "node_3_2_b_2", &err);
	cl_kernel node_4_kernel = clCreateKernel(program, "node_4", &err);
	
                      
        cl_mem in_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), in, NULL);	
        cl_mem out_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out, NULL);	
        cl_mem out_node_0_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_0, NULL);
	cl_mem out_node_1_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_1, NULL);
	cl_mem out_node_2_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_2, NULL);
	cl_mem out_node_3_1_a_1_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_3_1_a_1, NULL);
	cl_mem out_node_3_1_a_2_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_3_1_a_2, NULL);
	cl_mem out_node_3_1_b_1_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_3_1_b_1, NULL);
	cl_mem out_node_3_1_b_2_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_3_1_b_2, NULL);
	cl_mem out_node_3_2_a_1_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_3_2_a_1, NULL);
	cl_mem out_node_3_2_a_2_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_3_2_a_2, NULL);
	cl_mem out_node_3_2_b_1_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_3_2_b_1, NULL);
	cl_mem out_node_3_2_b_2_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_3_2_b_2, NULL);
	cl_mem out_node_4_buf = clCreateBuffer(context,  CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, N * sizeof(cl_float), out_node_4, NULL);

    cl_mem all_buffers[] = {in_buf, out_buf, out_node_0_buf, out_node_1_buf, out_node_2_buf, 
        out_node_3_1_a_1_buf,
        out_node_3_1_a_2_buf,
        out_node_3_1_b_1_buf,
        out_node_3_1_b_2_buf,
        out_node_3_2_a_1_buf,
        out_node_3_2_a_2_buf,
        out_node_3_2_b_1_buf,
        out_node_3_2_b_2_buf,
        };

                      
        err  = clSetKernelArg(node_0_kernel, 0, sizeof(cl_mem), &in_buf);
	err  = clSetKernelArg(node_0_kernel, 1, sizeof(cl_mem), &out_node_0_buf);
	err  = clSetKernelArg(node_0_kernel, 2, sizeof(cl_int), &iters_node_0);
	err  = clSetKernelArg(node_1_kernel, 0, sizeof(cl_mem), &out_node_0_buf);
	err  = clSetKernelArg(node_1_kernel, 1, sizeof(cl_mem), &out_node_1_buf);
	err  = clSetKernelArg(node_1_kernel, 2, sizeof(cl_int), &iters_node_1);
	err  = clSetKernelArg(node_2_kernel, 0, sizeof(cl_mem), &out_node_1_buf);
	err  = clSetKernelArg(node_2_kernel, 1, sizeof(cl_mem), &out_node_2_buf);
	err  = clSetKernelArg(node_2_kernel, 2, sizeof(cl_int), &iters_node_2);

	err  = clSetKernelArg(node_3_1_a_1_kernel, 0, sizeof(cl_mem), &out_node_0_buf);
	err  = clSetKernelArg(node_3_1_a_1_kernel, 1, sizeof(cl_mem), &out_node_3_1_a_1_buf);
	err  = clSetKernelArg(node_3_1_a_1_kernel, 2, sizeof(cl_int), &iters_node_3);
	err  = clSetKernelArg(node_3_1_a_2_kernel, 0, sizeof(cl_mem), &out_node_3_1_a_1_buf);
	err  = clSetKernelArg(node_3_1_a_2_kernel, 1, sizeof(cl_mem), &out_node_3_1_a_2_buf);
	err  = clSetKernelArg(node_3_1_a_2_kernel, 2, sizeof(cl_int), &iters_node_3);
	err  = clSetKernelArg(node_3_1_b_1_kernel, 0, sizeof(cl_mem), &out_node_3_1_a_1_buf);
	err  = clSetKernelArg(node_3_1_b_1_kernel, 1, sizeof(cl_mem), &out_node_3_1_b_1_buf);
	err  = clSetKernelArg(node_3_1_b_1_kernel, 2, sizeof(cl_int), &iters_node_3);
	err  = clSetKernelArg(node_3_1_b_2_kernel, 0, sizeof(cl_mem), &out_node_3_1_b_1_buf);
	err  = clSetKernelArg(node_3_1_b_2_kernel, 1, sizeof(cl_mem), &out_node_3_1_b_2_buf);
	err  = clSetKernelArg(node_3_1_b_2_kernel, 2, sizeof(cl_int), &iters_node_3);

	err  = clSetKernelArg(node_3_2_a_1_kernel, 0, sizeof(cl_mem), &out_node_0_buf);
	err  = clSetKernelArg(node_3_2_a_1_kernel, 1, sizeof(cl_mem), &out_node_3_2_a_1_buf);
	err  = clSetKernelArg(node_3_2_a_1_kernel, 2, sizeof(cl_int), &iters_node_3);
	err  = clSetKernelArg(node_3_2_a_2_kernel, 0, sizeof(cl_mem), &out_node_3_2_a_1_buf);
	err  = clSetKernelArg(node_3_2_a_2_kernel, 1, sizeof(cl_mem), &out_node_3_2_a_2_buf);
	err  = clSetKernelArg(node_3_2_a_2_kernel, 2, sizeof(cl_int), &iters_node_3);
	err  = clSetKernelArg(node_3_2_b_1_kernel, 0, sizeof(cl_mem), &out_node_3_2_a_1_buf);
	err  = clSetKernelArg(node_3_2_b_1_kernel, 1, sizeof(cl_mem), &out_node_3_2_b_1_buf);
	err  = clSetKernelArg(node_3_2_b_1_kernel, 2, sizeof(cl_int), &iters_node_3);
	err  = clSetKernelArg(node_3_2_b_2_kernel, 0, sizeof(cl_mem), &out_node_3_1_b_1_buf);
	err  = clSetKernelArg(node_3_2_b_2_kernel, 1, sizeof(cl_mem), &out_node_3_2_b_2_buf);
	err  = clSetKernelArg(node_3_2_b_2_kernel, 2, sizeof(cl_int), &iters_node_3);

	err  = clSetKernelArg(node_4_kernel, 0, sizeof(cl_mem), &out_node_2_buf);
	err  = clSetKernelArg(node_4_kernel, 1, sizeof(cl_mem), &out_node_3_1_b_2_buf);
	err  = clSetKernelArg(node_4_kernel, 2, sizeof(cl_mem), &out_node_3_2_b_2_buf);
	err  = clSetKernelArg(node_4_kernel, 3, sizeof(cl_mem), &out_buf);
	err  = clSetKernelArg(node_4_kernel, 4, sizeof(cl_int), &iters_node_4);

    err = clEnqueueMigrateMemObjects(commands, 13, all_buffers, 0, 0, NULL, NULL);
    clFinish(commands);
	
                      
        cl_event node_0_done[n_iters];
	cl_event node_1_done[n_iters];
	cl_event node_2_done[n_iters];

#define Di_ITERS 8

	cl_event node_3_1_a_1_done[n_iters][Di_ITERS];
	cl_event node_3_1_a_2_done[n_iters][Di_ITERS];
	cl_event node_3_1_b_1_done[n_iters][Di_ITERS];
	cl_event node_3_1_b_2_done[n_iters][Di_ITERS];
	cl_event node_3_2_a_1_done[n_iters][Di_ITERS];
	cl_event node_3_2_a_2_done[n_iters][Di_ITERS];
	cl_event node_3_2_b_1_done[n_iters][Di_ITERS];
	cl_event node_3_2_b_2_done[n_iters][Di_ITERS];
	cl_event node_4_done[n_iters];
	
                      
        double start = omp_get_wtime();

	// graph_iter == 0
	int graph_iter = 0;
	err = clEnqueueTask(commands, node_0_kernel, 0, NULL, &node_0_done[0]);
	cl_event node_1_waitlist[] = {node_0_done[0]};
	err = clEnqueueTask(commands, node_1_kernel, 1, node_1_waitlist, &node_1_done[0]);
	cl_event node_2_waitlist[] = {node_1_done[0]};
	err = clEnqueueTask(commands, node_2_kernel, 1, node_2_waitlist, &node_2_done[0]);


	// i = 0
	cl_event node_3_1_a_1_waitlist[] = {node_0_done[graph_iter]};
	err = clEnqueueTask(commands, node_3_1_a_1_kernel, 1, node_3_1_a_1_waitlist, &node_3_1_a_1_done[graph_iter][0]);
	cl_event node_3_1_a_2_waitlist[] = {node_3_1_a_1_done[graph_iter][0], node_3_1_a_2_done[graph_iter-1][0]};
	err = clEnqueueTask(commands, node_3_1_a_2_kernel, 1, node_3_1_a_2_waitlist, &node_3_1_a_2_done[graph_iter][0]);
	cl_event node_3_1_b_1_waitlist[] = {node_3_1_a_1_done[graph_iter][0], node_3_1_b_1_done[graph_iter-1][0]};
	err = clEnqueueTask(commands, node_3_1_b_1_kernel, 1, node_3_1_b_1_waitlist, &node_3_1_b_1_done[graph_iter][0]);
	cl_event node_3_1_b_2_waitlist[] = {node_3_1_b_1_done[graph_iter][0], node_3_1_b_2_done[graph_iter-1][0]};
	err = clEnqueueTask(commands, node_3_1_b_2_kernel, 1, node_3_1_b_2_waitlist, &node_3_1_b_2_done[graph_iter][0]);
	cl_event node_3_2_a_1_waitlist[] = {node_0_done[graph_iter], node_3_2_a_1_done[graph_iter-1][0]};
	err = clEnqueueTask(commands, node_3_2_a_1_kernel, 1, node_3_2_a_1_waitlist, &node_3_2_a_1_done[graph_iter][0]);
	cl_event node_3_2_a_2_waitlist[] = {node_3_2_a_1_done[graph_iter][0], node_3_2_a_2_done[graph_iter-1][0]};
	err = clEnqueueTask(commands, node_3_2_a_2_kernel, 1, node_3_2_a_2_waitlist, &node_3_2_a_2_done[graph_iter][0]);
	cl_event node_3_2_b_1_waitlist[] = {node_3_2_a_1_done[graph_iter][0], node_3_2_b_1_done[graph_iter-1][0]};
	err = clEnqueueTask(commands, node_3_2_b_1_kernel, 1, node_3_2_b_1_waitlist, &node_3_2_b_1_done[graph_iter][0]);
	cl_event node_3_2_b_2_waitlist[] = {node_3_2_b_1_done[graph_iter][0], node_3_2_b_2_done[graph_iter-1][0]};
	err = clEnqueueTask(commands, node_3_2_b_2_kernel, 1, node_3_2_b_2_waitlist, &node_3_2_b_2_done[graph_iter][0]);

	for(int i = 1; i < Di_ITERS; i++) {
	    cl_event node_3_1_a_1_waitlist[] = {node_3_1_a_2_done[graph_iter][i-1], node_3_1_a_1_done[graph_iter][i-1]};
	    err = clEnqueueTask(commands, node_3_1_a_1_kernel, 2, node_3_1_a_1_waitlist, &node_3_1_a_1_done[graph_iter][i]);
	    cl_event node_3_1_a_2_waitlist[] = {node_3_1_a_1_done[graph_iter][i], node_3_1_a_2_done[graph_iter][i-1]};
	    err = clEnqueueTask(commands, node_3_1_a_2_kernel, 2, node_3_1_a_2_waitlist, &node_3_1_a_2_done[graph_iter][i]);
	    cl_event node_3_1_b_1_waitlist[] = {node_3_1_a_1_done[graph_iter][i], node_3_1_b_1_done[graph_iter][i-1]};
	    err = clEnqueueTask(commands, node_3_1_b_1_kernel, 2, node_3_1_b_1_waitlist, &node_3_1_b_1_done[graph_iter][i]);
	    cl_event node_3_1_b_2_waitlist[] = {node_3_1_b_1_done[graph_iter][i], node_3_1_b_2_done[graph_iter][i-1]};
	    err = clEnqueueTask(commands, node_3_1_b_2_kernel, 2, node_3_1_b_2_waitlist, &node_3_1_b_2_done[graph_iter][i]);
	    cl_event node_3_2_a_1_waitlist[] = {node_3_2_a_2_done[graph_iter][i-1], node_3_2_a_1_done[graph_iter][i-1]};
	    err = clEnqueueTask(commands, node_3_2_a_1_kernel, 2, node_3_2_a_1_waitlist, &node_3_2_a_1_done[graph_iter][i]);
	    cl_event node_3_2_a_2_waitlist[] = {node_3_2_a_1_done[graph_iter][i], node_3_2_a_2_done[graph_iter][i-1]};
	    err = clEnqueueTask(commands, node_3_2_a_2_kernel, 2, node_3_2_a_2_waitlist, &node_3_2_a_2_done[graph_iter][i]);
	    cl_event node_3_2_b_1_waitlist[] = {node_3_2_a_1_done[graph_iter][i-1], node_3_2_b_2_done[graph_iter][i-1]};
	    err = clEnqueueTask(commands, node_3_2_b_1_kernel, 2, node_3_2_b_1_waitlist, &node_3_2_b_1_done[graph_iter][i]);
	    cl_event node_3_2_b_2_waitlist[] = {node_3_2_b_1_done[graph_iter][i], node_3_2_b_2_done[graph_iter][i-1]};
	    err = clEnqueueTask(commands, node_3_2_b_2_kernel, 2, node_3_2_b_2_waitlist, &node_3_2_b_2_done[graph_iter][i]);
	}
	cl_event node_4_waitlist[] = {node_2_done[graph_iter], node_3_1_b_2_done[graph_iter][Di_ITERS-1], node_3_2_b_2_done[graph_iter][Di_ITERS-1]};
	err = clEnqueueTask(commands, node_4_kernel, 3, node_4_waitlist, &node_4_done[graph_iter]);
	if(!pipelined)
		clFinish(commands);


	for(graph_iter = 1; graph_iter < n_iters; graph_iter++) {
		cl_event node_0_waitlist[] = {node_0_done[graph_iter-1]};
		err = clEnqueueTask(commands, node_0_kernel, 1, node_0_waitlist, &node_0_done[graph_iter]);
		cl_event node_1_waitlist[] = {node_0_done[graph_iter], node_1_done[graph_iter-1]};
		err = clEnqueueTask(commands, node_1_kernel, 2, node_1_waitlist, &node_1_done[graph_iter]);
		cl_event node_2_waitlist[] = {node_1_done[graph_iter], node_2_done[graph_iter-1]};
		err = clEnqueueTask(commands, node_2_kernel, 2, node_2_waitlist, &node_2_done[graph_iter]);

		// i = 0
		cl_event node_3_1_a_1_waitlist[] = {node_0_done[graph_iter], node_3_1_a_1_done[graph_iter-1][Di_ITERS-1], node_3_1_a_2_done[graph_iter-1][Di_ITERS-1]};
		err = clEnqueueTask(commands, node_3_1_a_1_kernel, 3, node_3_1_a_1_waitlist, &node_3_1_a_1_done[graph_iter][0]);
		cl_event node_3_1_a_2_waitlist[] = {node_3_1_a_1_done[graph_iter][0], node_3_1_a_2_done[graph_iter-1][0]};
		err = clEnqueueTask(commands, node_3_1_a_2_kernel, 2, node_3_1_a_2_waitlist, &node_3_1_a_2_done[graph_iter][0]);
		cl_event node_3_1_b_1_waitlist[] = {node_3_1_a_1_done[graph_iter][0], node_3_1_b_1_done[graph_iter-1][0]};
		err = clEnqueueTask(commands, node_3_1_b_1_kernel, 2, node_3_1_b_1_waitlist, &node_3_1_b_1_done[graph_iter][0]);
		cl_event node_3_1_b_2_waitlist[] = {node_3_1_b_1_done[graph_iter][0], node_3_1_b_2_done[graph_iter-1][0]};
		err = clEnqueueTask(commands, node_3_1_b_2_kernel, 2, node_3_1_b_2_waitlist, &node_3_1_b_2_done[graph_iter][0]);
		cl_event node_3_2_a_1_waitlist[] = {node_0_done[graph_iter], node_3_2_a_1_done[graph_iter-1][Di_ITERS-1], node_3_2_a_2_done[graph_iter-1][Di_ITERS-1]};
		err = clEnqueueTask(commands, node_3_2_a_1_kernel, 3, node_3_2_a_1_waitlist, &node_3_2_a_1_done[graph_iter][0]);
		cl_event node_3_2_a_2_waitlist[] = {node_3_2_a_1_done[graph_iter][0], node_3_2_a_2_done[graph_iter-1][0]};
		err = clEnqueueTask(commands, node_3_2_a_2_kernel, 2, node_3_2_a_2_waitlist, &node_3_2_a_2_done[graph_iter][0]);
		cl_event node_3_2_b_1_waitlist[] = {node_3_2_a_1_done[graph_iter][0], node_3_2_b_1_done[graph_iter-1][0]};
		err = clEnqueueTask(commands, node_3_2_b_1_kernel, 2, node_3_2_b_1_waitlist, &node_3_2_b_1_done[graph_iter][0]);
		cl_event node_3_2_b_2_waitlist[] = {node_3_2_b_1_done[graph_iter][0], node_3_2_b_2_done[graph_iter-1][0]};
		err = clEnqueueTask(commands, node_3_2_b_2_kernel, 2, node_3_2_b_2_waitlist, &node_3_2_b_2_done[graph_iter][0]);

		for(int i = 1; i < Di_ITERS; i++) {
		    cl_event node_3_1_a_1_waitlist[] = {node_3_1_a_2_done[graph_iter][i-1], node_3_1_a_1_done[graph_iter][i-1], node_3_1_a_1_done[graph_iter-1][i-1]};
		    err = clEnqueueTask(commands, node_3_1_a_1_kernel, 3, node_3_1_a_1_waitlist, &node_3_1_a_1_done[graph_iter][i]);
		    cl_event node_3_1_a_2_waitlist[] = {node_3_1_a_1_done[graph_iter][i], node_3_1_a_2_done[graph_iter][i-1], node_3_1_a_2_done[graph_iter-1][i-1]};
		    err = clEnqueueTask(commands, node_3_1_a_2_kernel, 3, node_3_1_a_2_waitlist, &node_3_1_a_2_done[graph_iter][i]);
		    cl_event node_3_1_b_1_waitlist[] = {node_3_1_a_1_done[graph_iter][i], node_3_1_b_1_done[graph_iter][i-1], node_3_1_b_1_done[graph_iter-1][i-1]};
		    err = clEnqueueTask(commands, node_3_1_b_1_kernel, 3, node_3_1_b_1_waitlist, &node_3_1_b_1_done[graph_iter][i]);
		    cl_event node_3_1_b_2_waitlist[] = {node_3_1_b_1_done[graph_iter][i], node_3_1_b_2_done[graph_iter][i-1], node_3_1_b_2_done[graph_iter-1][i-1]};
		    err = clEnqueueTask(commands, node_3_1_b_2_kernel, 3, node_3_1_b_2_waitlist, &node_3_1_b_2_done[graph_iter][i]);
		    cl_event node_3_2_a_1_waitlist[] = {node_3_2_a_2_done[graph_iter][i-1], node_3_2_a_1_done[graph_iter][i-1], node_3_2_a_1_done[graph_iter-1][i-1]};
		    err = clEnqueueTask(commands, node_3_2_a_1_kernel, 3, node_3_2_a_1_waitlist, &node_3_2_a_1_done[graph_iter][i]);
		    cl_event node_3_2_a_2_waitlist[] = {node_3_2_a_1_done[graph_iter][i], node_3_2_a_2_done[graph_iter][i-1], node_3_2_a_2_done[graph_iter-1][i-1]};
		    err = clEnqueueTask(commands, node_3_2_a_2_kernel, 3, node_3_2_a_2_waitlist, &node_3_2_a_2_done[graph_iter][i]);
		    cl_event node_3_2_b_1_waitlist[] = {node_3_2_a_1_done[graph_iter][i-1], node_3_2_b_2_done[graph_iter][i-1], node_3_2_b_1_done[graph_iter-1][i-1]};
		    err = clEnqueueTask(commands, node_3_2_b_1_kernel, 3, node_3_2_b_1_waitlist, &node_3_2_b_1_done[graph_iter][i]);
		    cl_event node_3_2_b_2_waitlist[] = {node_3_2_b_1_done[graph_iter][i], node_3_2_b_2_done[graph_iter][i-1], node_3_2_b_2_done[graph_iter-1][i-1]};
		    err = clEnqueueTask(commands, node_3_2_b_2_kernel, 3, node_3_2_b_2_waitlist, &node_3_2_b_2_done[graph_iter][i]);
		}


		cl_event node_4_waitlist[] = {node_2_done[graph_iter], node_3_1_b_2_done[graph_iter][Di_ITERS-1], node_3_2_b_2_done[graph_iter][Di_ITERS-1]};
		err = clEnqueueTask(commands, node_4_kernel, 3, node_4_waitlist, &node_4_done[graph_iter]);

		if(!pipelined)
			clFinish(commands);
	}
	
                      
        clFinish(commands);
        double exec_time = omp_get_wtime() - start;
        printf("Execution time: %lf\n", exec_time); 
                      
        err = clEnqueueReadBuffer( commands, out_buf, CL_TRUE, 0, N * sizeof(float), out, 0, NULL, NULL );      
                      
        for(int i = 0; i < N; i++) {
            printf("out[%d] = %f\n", i, out[i]);
            out[i] = 0;
        }             
    }                 
