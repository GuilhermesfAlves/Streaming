# Trabalho 1 da matéria Redes de Computadores I
Autor: Guilherme dos Santos Ferreira Alves

## Streaming de video.

Modo de uso:
Você necessita estar em um ambiente linux.
Você necessita ter acesso ao Root do sistema.

``` bash
make
```

### Web interface:
Execute `ifconfig` para saber quais interfaces de rede estão disponíveis na sua máquina
``` bash
ifconfig
```
Exemplos de interfaces de rede: `lo`, `enp3s0`, `enp0s31f6`, ...

#### Loopback
Use `lo` se você não estiver conectando duas máquinas através de um cabo.

Primeiro você deve inicializar em um terminal o servidor.
```bash
sudo ./exec server [Web interface]
```

Depois você deverá inicializar o client.
```bash
sudo ./exec client [Web interface]
```

Assim você estabelecerá a conexão entre eles.


Mensagem à ser transmitida:

| Marcador de Inicio | Tamanho em bytes | Sequência | Tipo | Dados | CRC-8 |
|:------------------:|:----------------:|:---------:|:----:|:-----:|:-----:|
| 8 bits             | 6 bits           | 5 bits    | 5 bits| 0-63 bytes | 8 bits |
| 1 byte             |        ->        | 2 bytes   |  <-   | 0-63 bytes | 1 byte |


O marcador de inicio sempre será `0111 1110`

O campo tipo pode ser:

| bits | código | relação |
|:----------:|:---:|:-----:|
|`00000` | ack | servidor <-> cliente |
|`00001` | nack | servidor <-> cliente |
|`01010` | lista | servidor <-> cliente | Cliente pede ao servidor a lista, com list, e o servidor retorna a quantidade de elementos com list
|`01011` | baixar | servidor <- cliente | Cliente requisita o download de tal arquivo
|`10000` | mostra na tela | servidor -> cliente | Ao fim da transmissão dos dados do arquivo se manda o comando para se executar um player para executar tal arquivo
|`10001` | descritor arquivo | servidor -> cliente | Após o servidor receber o pedido de download, ele retorna o tamanho do arquivo
|`10010` | dados | servidor -> cliente | 
|`11110` | fim tx | servidor <- cliente |
|`11111` | erro | servidor <-> cliente |

Os campos de erro podem estar com a seguinte mensagem em dados:

| erro |  definição | relação | 
|:----:|:----------:|:---------:|
| `1` | acesso negado | servidor ? cliente |
| `2` | não encontrado| servidor -> cliente |
| `3` | disco cheio   | servidor <- cliente |