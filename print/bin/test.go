package main

import (
    "fmt"
    "os/exec"
	"time"
)

func main(){

for{
cmd := exec.Command("./lkm_print")

    err := cmd.Run()
    if err != nil {
        fmt.Println("Execute Command failed:" + err.Error())
        return
    }
    fmt.Println("lkm print loaded.")
    time.Sleep(time.Duration(5)*time.Second)
}
    //fmt.Println("Execute Command finished.")
}

