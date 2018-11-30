# 第一步 编译TensorFlow
`./configure`

`bazel build -c opt //tensorflow:libtensorflow.so`

导入环境变量
`export LD_LIBRARY_PATH=${GOPATH}/src/github.com/tensorflow/tensorflow/bazel-bin/tensorflow`

go test

`go test github.com/tensorflow/tensorflow/tensorflow/go`