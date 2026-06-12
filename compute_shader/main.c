#include <SDL3/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#define ELEMENT_COUNT 8
#define WORKGROUP_SIZE 64

static Uint8 *load_file_or_fail(const char *path, size_t *size) {
  Uint8 *data = SDL_LoadFile(path, size);
  if (!data) {
    SDL_Log("Failed to load %s: %s", path, SDL_GetError());
  }
  return data;
}

static SDL_GPUBuffer *create_buffer_or_fail(SDL_GPUDevice *device,
                                            SDL_GPUBufferUsageFlags usage,
                                            Uint32 size) {
  SDL_GPUBufferCreateInfo info = {
      .usage = usage,
      .size = size,
  };

  SDL_GPUBuffer *buffer = SDL_CreateGPUBuffer(device, &info);
  if (!buffer) {
    SDL_Log("Failed to create GPU buffer: %s", SDL_GetError());
  }
  return buffer;
}

static SDL_GPUTransferBuffer *
create_transfer_or_fail(SDL_GPUDevice *device, SDL_GPUTransferBufferUsage usage,
                        Uint32 size) {
  SDL_GPUTransferBufferCreateInfo info = {
      .usage = usage,
      .size = size,
  };

  SDL_GPUTransferBuffer *buffer = SDL_CreateGPUTransferBuffer(device, &info);
  if (!buffer) {
    SDL_Log("Failed to create GPU transfer buffer: %s", SDL_GetError());
  }
  return buffer;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  const int a[ELEMENT_COUNT] = {1, 2, 3, 4, 5, 6, 7, 8};
  const int b[ELEMENT_COUNT] = {10, 20, 30, 40, 50, 60, 70, 80};
  const Uint32 byte_count = (Uint32)(sizeof(int) * ELEMENT_COUNT);

  int exit_code = 1;
  Uint8 *shader_code = NULL;
  size_t shader_size = 0;
  SDL_GPUDevice *device = NULL;
  SDL_GPUComputePipeline *pipeline = NULL;
  SDL_GPUBuffer *a_gpu = NULL;
  SDL_GPUBuffer *b_gpu = NULL;
  SDL_GPUBuffer *out_gpu = NULL;
  SDL_GPUTransferBuffer *upload = NULL;
  SDL_GPUTransferBuffer *download = NULL;
  SDL_GPUFence *fence = NULL;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_Init failed: %s", SDL_GetError());
    goto cleanup;
  }

  device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, "vulkan");
  if (!device) {
    SDL_Log("Failed to create Vulkan GPU device: %s", SDL_GetError());
    goto cleanup;
  }

  shader_code = load_file_or_fail("sum.comp.spv", &shader_size);
  if (!shader_code) {
    goto cleanup;
  }

  SDL_GPUComputePipelineCreateInfo pipeline_info = {
      .code_size = shader_size,
      .code = shader_code,
      .entrypoint = "main",
      .format = SDL_GPU_SHADERFORMAT_SPIRV,
      .num_readonly_storage_buffers = 2,
      .num_readwrite_storage_buffers = 1,
      .threadcount_x = WORKGROUP_SIZE,
      .threadcount_y = 1,
      .threadcount_z = 1,
  };

  pipeline = SDL_CreateGPUComputePipeline(device, &pipeline_info);
  if (!pipeline) {
    SDL_Log("Failed to create compute pipeline: %s", SDL_GetError());
    goto cleanup;
  }

  a_gpu = create_buffer_or_fail(
      device, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ, byte_count);
  b_gpu = create_buffer_or_fail(
      device, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ, byte_count);
  out_gpu = create_buffer_or_fail(
      device, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE, byte_count);
  upload = create_transfer_or_fail(device, SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
                                   byte_count * 2);
  download = create_transfer_or_fail(
      device, SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD, byte_count);
  if (!a_gpu || !b_gpu || !out_gpu || !upload || !download) {
    goto cleanup;
  }

  void *upload_data = SDL_MapGPUTransferBuffer(device, upload, false);
  if (!upload_data) {
    SDL_Log("Failed to map upload buffer: %s", SDL_GetError());
    goto cleanup;
  }
  SDL_memcpy(upload_data, a, byte_count);
  SDL_memcpy((Uint8 *)upload_data + byte_count, b, byte_count);
  SDL_UnmapGPUTransferBuffer(device, upload);

  SDL_GPUCommandBuffer *commands = SDL_AcquireGPUCommandBuffer(device);
  if (!commands) {
    SDL_Log("Failed to acquire command buffer: %s", SDL_GetError());
    goto cleanup;
  }

  SDL_GPUCopyPass *upload_pass = SDL_BeginGPUCopyPass(commands);
  SDL_GPUTransferBufferLocation upload_a = {.transfer_buffer = upload,
                                            .offset = 0};
  SDL_GPUTransferBufferLocation upload_b = {.transfer_buffer = upload,
                                            .offset = byte_count};
  SDL_GPUBufferRegion a_region = {
      .buffer = a_gpu, .offset = 0, .size = byte_count};
  SDL_GPUBufferRegion b_region = {
      .buffer = b_gpu, .offset = 0, .size = byte_count};
  SDL_UploadToGPUBuffer(upload_pass, &upload_a, &a_region, false);
  SDL_UploadToGPUBuffer(upload_pass, &upload_b, &b_region, false);
  SDL_EndGPUCopyPass(upload_pass);

  SDL_GPUStorageBufferReadWriteBinding output_binding = {
      .buffer = out_gpu,
      .cycle = false,
  };
  SDL_GPUComputePass *compute_pass =
      SDL_BeginGPUComputePass(commands, NULL, 0, &output_binding, 1);
  SDL_BindGPUComputePipeline(compute_pass, pipeline);
  SDL_GPUBuffer *inputs[] = {a_gpu, b_gpu};
  SDL_BindGPUComputeStorageBuffers(compute_pass, 0, inputs,
                                   SDL_arraysize(inputs));
  SDL_DispatchGPUCompute(compute_pass, 1, 1, 1);
  SDL_EndGPUComputePass(compute_pass);

  SDL_GPUCopyPass *download_pass = SDL_BeginGPUCopyPass(commands);
  SDL_GPUBufferRegion out_region = {
      .buffer = out_gpu, .offset = 0, .size = byte_count};
  SDL_GPUTransferBufferLocation download_location = {
      .transfer_buffer = download, .offset = 0};
  SDL_DownloadFromGPUBuffer(download_pass, &out_region, &download_location);
  SDL_EndGPUCopyPass(download_pass);

  fence = SDL_SubmitGPUCommandBufferAndAcquireFence(commands);
  if (!fence) {
    SDL_Log("Failed to submit command buffer: %s", SDL_GetError());
    goto cleanup;
  }
  if (!SDL_WaitForGPUFences(device, true, &fence, 1)) {
    SDL_Log("Failed while waiting for GPU fence: %s", SDL_GetError());
    goto cleanup;
  }

  int *result = SDL_MapGPUTransferBuffer(device, download, false);
  if (!result) {
    SDL_Log("Failed to map download buffer: %s", SDL_GetError());
    goto cleanup;
  }

  for (int i = 0; i < ELEMENT_COUNT; i++) {
    printf("%d + %d = %d\n", a[i], b[i], result[i]);
  }

  SDL_UnmapGPUTransferBuffer(device, download);
  exit_code = 0;

cleanup:
  if (fence) {
    SDL_ReleaseGPUFence(device, fence);
  }
  if (download) {
    SDL_ReleaseGPUTransferBuffer(device, download);
  }
  if (upload) {
    SDL_ReleaseGPUTransferBuffer(device, upload);
  }
  if (out_gpu) {
    SDL_ReleaseGPUBuffer(device, out_gpu);
  }
  if (b_gpu) {
    SDL_ReleaseGPUBuffer(device, b_gpu);
  }
  if (a_gpu) {
    SDL_ReleaseGPUBuffer(device, a_gpu);
  }
  if (pipeline) {
    SDL_ReleaseGPUComputePipeline(device, pipeline);
  }
  if (shader_code) {
    SDL_free(shader_code);
  }
  if (device) {
    SDL_DestroyGPUDevice(device);
  }
  SDL_Quit();
  return exit_code;
}
