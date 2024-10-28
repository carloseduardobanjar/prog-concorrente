/* Extraído da documentação de Go */

package main

import (
	"fmt"
	"math"
)

func ehPrimo(n int) bool {
	if n <= 1 {
		return false
	}
	if n == 2 {
		return true
	}
	if n%2 == 0 {
		return false
	}
	for i := int(3); i <= int(math.Sqrt(float64(n))); i += 2 {
		if n%i == 0 {
			return false
		}
	}
	return true
}

func tarefa(jobs chan int, done chan bool, result chan int, i int) {
    for {
    // the more value will be false if jobs has been 
    // closed and all values in the channel have already been received
        j, more := <-jobs
        if more {
            // fmt.Printf("[thread %d] received number %d\n", i, j)
            if ehPrimo(j) {
                result <- 1
            }
        } else {
            // fmt.Println("received all jobs")
            done <- true
            return
        }
    }
}

func main() {
    N := 100
    M := 4

    jobs := make(chan int, N)
    done := make(chan bool)
    result := make(chan int, N)

    for i := 0; i < M; i++ {
        go tarefa(jobs, done, result, i) //dispara a goroutine
    }
    
    for j := 1; j <= N; j++ {
        jobs <- j
        //fmt.Println("sent job", j)
    }

    close(jobs)
    // fmt.Println("sent all jobs")

    for i := 0; i < M; i++ {
        <-done // await the worker using the channel synchronization approach
    }

    fmt.Printf("Quantidade de primos %d\n", len(result))
}
