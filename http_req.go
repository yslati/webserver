package main

import(
	"io/ioutil"
	"log"
	"net/http"
	"fmt"
)
func test_req(url string) {
	resp, err := http.Get("http://127.0.0.1:" + url)

	if err != nil {
		log.Fatalln(err)
	}
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Println(string(body))
}

func main() {
	test_req("5000/dir")
}