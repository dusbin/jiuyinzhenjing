package retiver

//没有必要和interface中的一样
type Retriever struct {
	Url string
}
func (r *Retriever)Get()(url string){
	url = r.Url
	return
}
func (r *Retriever)Set(url string){
	r.Url = url
	return
}
func (r *Retriever)Add(url string)(ret string){
	r.Url = r.Url + url
	ret = r.Url
	return
}