# Wireshark 显示过滤器核心规则笔记

## 一、基本概念

- **作用**：在**已捕获**的数据包中，筛选出符合条件的数据包进行显示。它不会删除数据，只是隐藏不匹配的包。
- **输入位置**：主界面顶部工具栏的 `Apply a display filter...` 输入框。
- **语法反馈**：
  - **绿色背景**：语法正确且已匹配到数据。
  - **红色背景**：语法错误。
  - **白色背景**：语法正确但未匹配到任何数据。

## 二、核心语法规则

### 1. 比较运算符

| 运算符 | 含义     | 示例                          | 说明                      |
| :----- | :------- | :---------------------------- | :------------------------ |
| `==`   | 等于     | `ip.src == 192.168.1.1`       | 源 IP 是 192.168.1.1        |
| `!=`   | 不等于   | `tcp.port != 443`             | TCP 端口不是 443         |
| `>`    | 大于     | `frame.len > 1000`            | 帧长度大于 1000 字节      |
| `<`    | 小于     | `udp.length < 128`            | UDP 长度小于 128 字节     |
| `>=`   | 大于等于 | `http.content_length >= 500`  | HTTP 内容长度 >= 500      |
| `<=`   | 小于等于 | `tcp.window_size <= 8192`     | TCP 窗口大小 <= 8192      |

### 2. 逻辑运算符

| 运算符   | 含义  | 示例                                            | 说明                         |
| :---- | :-- | :-------------------------------------------- | :------------------------- |
| `and` | 与   | `ip.src == 192.168.1.1 and tcp.dstport == 80` | **同时满足**两个条件 (源IP且目标端口是80) |
| `or`  | 或   | `http or dns`                                 | 满足**任意一个**条件 (HTTP或DNS协议)  |
| `not` | 非   | `not arp`                                     | **排除**某个条件 (显示所有非ARP流量)    |
| `&&`  | 与   | 同 `and`                                       |                            |
| \|\|  | 或   | 同 `or`                                        |                            |
| `!`   | 非   | 同 `not`                                       |                            |
## 三、常用过滤器字段

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `frame.number` | 帧编号 | `frame.number == 100` |
| `frame.time` | 数据包捕获时间 | `frame.time >= "2023-10-01"` |
| `frame.len` | 帧长度 | `frame.len > 1500` |
| `frame.protocols` | 帧中包含的协议 | `frame.protocols contains "tcp"` |

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `ip.addr` | IP地址（源或目标） | `ip.addr == 192.168.1.1` |
| `ip.src` | 源IP地址 | `ip.src == 192.168.1.100` |
| `ip.dst` | 目标IP地址 | `ip.dst == 8.8.8.8` |
| `ip.ttl` | TTL值 | `ip.ttl < 64` |
| `ip.version` | IP版本 | `ip.version == 6` |

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `tcp.port` | TCP端口（源或目标） | `tcp.port == 80` |
| `tcp.srcport` | 源TCP端口 | `tcp.srcport == 443` |
| `tcp.dstport` | 目标TCP端口 | `tcp.dstport == 22` |
| `tcp.flags.syn` | SYN标志位 | `tcp.flags.syn == 1` |
| `tcp.flags.ack` | ACK标志位 | `tcp.flags.ack == 1` |
| `tcp.window_size` | 窗口大小 | `tcp.window_size < 8192` |
| `tcp.analysis.retransmission` | TCP重传 | `tcp.analysis.retransmission` |

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `udp.port` | UDP端口（源或目标） | `udp.port == 53` |
| `udp.srcport` | 源UDP端口 | `udp.srcport == 123` |
| `udp.dstport` | 目标UDP端口 | `udp.dstport == 67` |
| `udp.length` | UDP长度 | `udp.length > 100` |

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `icmp.type` | ICMP类型 | `icmp.type == 8` |
| `icmp.code` | ICMP代码 | `icmp.code == 0` |
| `icmp.ident` | ICMP标识符 | `icmp.ident == 0x1234` |

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `http` | HTTP协议 | `http` |
| `http.request` | HTTP请求 | `http.request` |
| `http.response` | HTTP响应 | `http.response` |
| `http.request.method` | HTTP方法 | `http.request.method == "GET"` |
| `http.response.code` | 状态码 | `http.response.code == 404` |
| `http.host` | 主机头 | `http.host contains "baidu"` |
| `http.user_agent` | User-Agent | `http.user_agent contains "Chrome"` |
| `http.content_type` | 内容类型 | `http.content_type == "text/html"` |

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `dns` | DNS协议 | `dns` |
| `dns.qry.name` | 查询域名 | `dns.qry.name == "www.example.com"` |
| `dns.resp.name` | 响应域名 | `dns.resp.name contains "google"` |
| `dns.flags.response` | 响应标志 | `dns.flags.response == 0` |
| `dns.a` | A记录 | `dns.a` |

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `arp` | ARP协议 | `arp` |
| `arp.opcode` | 操作码 | `arp.opcode == 1` |
| `arp.src.hw_mac` | 源MAC地址 | `arp.src.hw_mac == 00:11:22:33:44:55` |
| `arp.dst.proto_ipv4` | 目标IPv4地址 | `arp.dst.proto_ipv4 == 192.168.1.1` |

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `ssl` | SSL协议 | `ssl` |
| `tls` | TLS协议 | `tls` |
| `ssl.handshake.type` | 握手类型 | `ssl.handshake.type == 1` |
| `tls.record.version` | TLS版本 | `tls.record.version == 0x0303` |

## 其他协议

| 字段名 | 说明 | 示例 |
| :--- | :--- | :--- |
| `ftp` | FTP协议 | `ftp` |
| `ssh` | SSH协议 | `ssh` |
| `smtp` | SMTP协议 | `smtp` |
| `dhcp` | DHCP协议 | `dhcp` |
| `bootp` | BOOTP协议 | `bootp` |
| `icmpv6` | ICMPv6协议 | `icmpv6` |

## 常用
```

# 基础过滤
ip.addr == 192.168.1.100          # 查看与该IP地址相关的所有流量
tcp.port == 80                    # 查看所有HTTP流量
ip.src == 10.0.0.1 and ip.dst == 8.8.8.8 # 查看从10.0.0.1到8.8.8.8的流量
not arp and not icmp              # 排除ARP和ICMP流量，专注于上层协议

# 协议分析
http.request                      # 只看HTTP请求
tcp.flags.syn == 1                # 只看TCP SYN包 (三次握手第一步)
tcp.analysis.retransmission       # 查找TCP重传包 (网络可能有问题)
http.response.code == 404         # 查找404错误的HTTP响应

# 高级组合
(ip.src == 192.168.1.1 and tcp.dstport == 443) or (ip.dst == 192.168.1.1 and tcp.srcport == 443) # 查看与192.168.1.1的所有HTTPS通信

```