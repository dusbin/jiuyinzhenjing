# 运行测试环境
虚拟机环境ubuntu16.04.4_64bits

处理器 4个

内存 4GB
# 测试结果-使用make(chan int,1024)优化
## 优化前 
Read done: 59.413715887s

Read done: 1m0.322047938s

Read done: 1m5.238709215s

Read done: 1m5.424182654s

InMemSort done: 1m17.961608884s

InMemSort done: 1m18.42110843s

InMemSort done: 1m19.333735887s

InMemSort done: 1m19.397993075s

Merge done: 9m52.406663742s

Merge done: 9m52.406279589s

Merge done: 9m52.406550386s

Merge done: 9m52.406613282s

## 优化后
Read done: 20.072806587s

Read done: 20.483447493s

Read done: 20.790749483s

Read done: 21.094490189s

InMemSort done: 34.566434656s

InMemSort done: 34.628284465s

InMemSort done: 34.755828594s

InMemSort done: 34.783957196s

Merge done: 3m49.782084746s

Merge done: 3m49.781954563s

Merge done: 3m49.78559273s

Merge done: 3m49.783330799s
## 优化后实体机运行（ThinkPad E470 windows10 I7 16G）
Read done: 7.3135637s

Read done: 7.4811153s

Read done: 7.6596375s

Read done: 7.7543841s

InMemSort done: 23.2997827s

InMemSort done: 23.3845559s

InMemSort done: 23.6209234s

InMemSort done: 23.7585566s

Merge done: 1m5.9416505s

Merge done: 1m5.9416505s

Merge done: 1m5.9416505s

Merge done: 1m5.9416505s