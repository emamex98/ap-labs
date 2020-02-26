package main

import (
  "fmt"
)


func main() {

  var input string
  fmt.Scanf("%s", &input)

  var s []int
  
  var counter = 1
  var acc = 1
  
  s = append(s, int(input[0]))
  
  for i:=1;  i<len(input); i++ {
    
    if contains(s, int(input[i])) == false {
      counter = counter + 1
    } else {
      if acc < counter {
        acc = counter
      }
      counter = 1
    }
    
    s = append(s, int(input[i]))
  }
  
  if acc < counter {
    acc = counter
  }
  
  fmt.Println(acc)

}

func contains(s []int, e int) bool {
    for _, a := range s {
        if a == e {
            return true
        }
    }
    return false
}
