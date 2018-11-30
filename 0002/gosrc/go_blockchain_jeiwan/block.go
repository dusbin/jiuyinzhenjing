package main
import(
	"bytes"
	"crypto/sha256"
	"strconv"
	"time"
)
type Block struct{
	Timestramp 		int64
	Data			[]byte
	PrevBlockHash	[]byte
	Hash			[]byte
}
func (b *Block)SetHash(){
	timestamp 	:= []byte(strconv.FormatInt(b.Timestramp,10))
	headers 	:= bytes.Join([][]byte{b.PrevBlockHash,b.Data,timestamp},[]byte{})
	hash		:= sha256.Sum256(headers)
	b.Hash		 = hash[:]
}