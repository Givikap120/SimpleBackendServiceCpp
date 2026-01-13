# Simple Backend Service (C++)

A lightweight backend service built with **C++**, demonstrating asynchronous job execution, REST and gRPC APIs, and thread-safe task handling.  

This repository is intended as a project for resume to demonstrate my skills.

## Features

- **Thread-safe Executor** for concurrent job processing  
- **Job queue with record tracking**, ensuring safe access across threads  
- Implemented two APIs: REST and gRPC to show my ability to work with both, though for this project gRPC is more fitting, since this service is meant to be internal
- Sample job handlers:  
  - `PrintHandler` – prints messages  
  - `QuadraticEquationHandler` – solves quadratic equations  
- Unit tests using **Google Test**  

## Tech Stack

- **C++20**  
- **CMake**
- **vcpkg**
- **gRPC** and **protobuf** for RPC communication  
- **nlohmann/json** for JSON handling  
- **httplib** for REST server  
- **Google Test** for unit testing  

## Building and Running

1. Clone the repository
2. Install all the required c++ components and tools:
   - A C++20 compatible compiler (e.g., GCC 10+, Clang 10+, MSVC 2019+)  
   - CMake 3.15+  
   - vcpkg for dependency management
3. Install dependencies using `vcpkg install`.
4. Configure CMake (replace with real path to vcpkg):  
   ```cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake```
	OR, you could set environment variable `VCPKG_ROOT` to point to your vcpkg installation, then simply run:  
   ```cmake -B build -S .```
5. Build the project:  
   ```cmake --build build```
6. Run the executable:  
   ```./build/SimpleBackendServiceCpp```

## Using the API

### REST Endpoints

- **Submit a job**: `POST /jobs/submit`  
  - Body (JSON):  
    ```json
    {
      "type": "quadratic_equation",
      "payload": {
        "a": 1,
        "b": -6,
        "c": 9
        }
    }
    ```
- **Get job status**: `GET /jobs/status/{jobId}`  
  - Response (JSON):
    ```json
    {
    "status": "Completed"
    }
    ```
- **Get job result**: `GET /jobs/result/{jobId}`  
  - Response (JSON):
    ```json
    {
    "result": {
        "roots": [
            3.0
        ]
    },
    "status": "Completed"
    }
    ```

All job IDs are `uint64_t`. Status can be: `Unknown`, `Pending`, `Running`, `Completed`, `Failed`.
Status `Unknown` means the job wasn't found.

### gRPC Service

- **Package**: `SimpleBackendServiceCppGrpc`
- **Service**: `JobService`  
- **RPCs**:  
  - `SubmitJob(SubmitJobRequest) returns (SubmitJobResponse)`  
  - `GetJobStatus(GetJobStatusRequest) returns (GetJobStatusResponse)`  
  - `GetJobResult(GetJobResultRequest) returns (GetJobResultResponse)`  

Example `SubmitJobRequest`:

```protobuf
message SubmitJobRequest {
    string type = 1;
    string payload = 2;
}
```

## Unit Testing

The project uses **Google Test** (`gtest`) to verify correctness of the executor, job handlers, and gRPC/REST APIs.

### Test Coverage

1. **Executor**
   - Correct handling of job success, failure, crash inside the job, or trying to run an unknown job type.
   - Correct registration of handlers and preserving correct order of execution:
    - Can't register the same handler twice
    - Can't register after starting the executor
    - Can't submit jobs before starting the executor or after it's shutdown
    - Starting twice or with 0 threads is not allowed

2. **Handlers**
   - `QuadraticEquationHandler`: verifies correct roots for various quadratic equations. Also for invalid or missing payload fields.

### Running Tests

From the main directory:

```bash
cd build
ctest --output-on-failure
```

Or use something like Visual Studio test explorer.

## Notes

 - Both REST and gRPC servers share the same thread-safe executor and job queue, ensuring consistent state and safe concurrent access.
 - The project demonstrates clean separation of concerns between job handling, submission service, and server layers.
