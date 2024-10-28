//introduz canais na aplicação Go
package main

import (
	"fmt"
)

func tarefa(str chan string) {
	var msg = ""
	msg = <-str
	fmt.Println("[Goroutine]", msg)
	str <- "Oi Main, bom dia, tudo bem?"
	msg = <-str
	fmt.Println("[Goroutine]", msg)
	str <- "Certo, entendido."
	msg = <-str //main indica que pode terminar
	fmt.Println("[Goroutine]",  "Finalizando")
	str <- "tambem finalizei"
}

func main() {
	var msg = "" //string vazia
	
	//cria um canal de comunicacao nao-bufferizado
	str := make(chan string)
	
	//cria um goroutine que executará a função 'tarefa'
	go tarefa(str)
	str <- "Olá, Goroutine, bom dia!"
	msg = <-str
	fmt.Println("[Main]", msg)
	str <- "Tudo bem! Vou terminar tá?"
	msg = <-str
	fmt.Println("[Main]", msg)
	str <- "pode terminar!"
	msg = <-str //goroutine indica que ja terminou
	fmt.Println("[Main]", "Finalizando")
}
/*
Main envia para Goroutine: "Olá, Goroutine, bom dia!" (impresso pela Goroutine)
Goroutine envia para Main: "Oi Main, bom dia, tudo bem?" (impresso pela Main)
Main envia para Goroutine: "Tudo bem! Vou terminar tá?" (impresso pela Goroutine)
Goroutine envia para Main: "Certo, entendido." (impresso pela Main)
Goroutine imprime: finalizando
Main imprime: finalizando
*/
