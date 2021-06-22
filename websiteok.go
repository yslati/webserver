package main

import (
        "fmt"
        "net/http"
)

func request() {
        _, err := http.Get("http://localhost:5000")
        if err != nil {
                fmt.Println(err.Error())
        }
}

func main() {
        for i := 0; i < 1000; i++ {
                request()
        }
        fmt.Scanln()
}
