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
rode `ifconfig` para saber quais interfaces de rede estão disponíveis na sua máquina
``` bash
ifconfig
```
Possíveis interfaces de rede: `lo`, `enp3s0`, `enp0s31f6` 

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

| bits | código |
|:----------:|:---:|
|`00000` | ack |
|`00001` | nack |
|`01010` | lista |
|`01011` | baixar |
|`10000` | mostra na tela |
|`10001` | descritor arquivo |
|`10010` | dados |
|`11110` | fim tx |
|`11111` | erro|

Os campos de erro podem estar com a seguinte mensagem em dados:

| erro |  definição |
|:----:|:----------:|
| `1` | acesso negado |
| `2` | não encontrado|
| `3` | disco cheio   |