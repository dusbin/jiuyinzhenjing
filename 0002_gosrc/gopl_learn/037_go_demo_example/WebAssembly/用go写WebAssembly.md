# Golang WebAssembly 入门
Golang 在1.11版本中引入了 WebAssembly 支持,意味着以后可以用 go编写可以在浏览器中运行的程序,当然这个肯定也是要受浏览器沙盒环境约束的.
1. code
    ```
    package main
    func main() {
        println("Hello, WebAssembly!")
    }
    ```
2. 编译

    必须是 go1.11才行

    `GOARCH=wasm GOOS=js go build -o test.wasm main.go`

3. 运行

    单独的 wasm 文件是无法直接运行的,必须载入浏览器中.

    ```
    mkdir test
    cp test.wasm test
    cp $GOROOT/misc/wasm/wasm_exec.{html,js} .

4. 一个测试 http 服务器

    chrome 是不支持本地文件中运行 wasm 的,所以必须有一个 http 服务器
    ```
    //http.go
    package main

    import (
        "flag"
        "log"
        "net/http"
        "strings"
    )

    var (
        listen = flag.String("listen", ":8080", "listen address")
        dir    = flag.String("dir", ".", "directory to serve")
    )

    func main() {
        flag.Parse()
        log.Printf("listening on %q...", *listen)
        log.Fatal(http.ListenAndServe(*listen, http.HandlerFunc(func(resp http.ResponseWriter, req *http.Request) {
            if strings.HasSuffix(req.URL.Path, ".wasm") {
                resp.Header().Set("content-type", "application/wasm")
            }

            http.FileServer(http.Dir(*dir)).ServeHTTP(resp, req)
        })))
    }

5. http.go
    ```
    mv http.go test
    cd test
    go run http.go 
    ```
6. 效果

    在浏览器中打开http://localhost:8080/wasm_exec.html,点击 run 按钮,可以在控制台看到 Hello, WebAssembly!字符串
7. node中运行 wasm

    这个更直接

    `node wasm_exec.js test.wasm`

    就可以在控制台看到Hello, WebAssembly!字符串了.