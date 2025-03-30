package main

import (
	"fmt"
	"net/http"

	"tinygo.org/x/drivers/netlink"
	"tinygo.org/x/drivers/netlink/probe"
)

func HelloServer(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hello, %s!", r.URL.Path[1:])
}

func main() {

	// load network driver for target
	link, _ := probe.Probe()

	// Connect target to IP network
	link.NetConnect(&netlink.ConnectParams{
		Ssid:       "my SSID",
		Passphrase: "my passphrase",
	})

	// Serve it up
	http.HandleFunc("/", HelloServer)
	http.ListenAndServe(":8080", nil)
}
