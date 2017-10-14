
# A Guide to Network Function Virtualization (NFV)

- Hiroki SHIROKURA
- slank.dev@gmail.com
- 2017.08.03

これは私なりの要約である

[Original Link (en)](https://www.fir3net.com/Networking/Concepts-and-Terminology/a-guide-to-network-function-virtualisation-nfv.html)

## Introduction

NFVは数年前からあった用語です. <br>
この記事では, NFVシステムを構築するために必要なさまざまな以下について説明する.
- 標準化
- プラットフォーム
- アクセラレーション技術
- etc..

## What is NFV?

NFVとは? <br>
NFVは, ファイアウォール, ルータ, ロードバランサなど物理ハードウェア上で
実行されているNFを汎用サーバ上の仮想化で実現する概念です.

## Advantages

NFVはOPEX(運用支出)とCAPEX(資本支出)の両面で数多くの利点をもたらします.

CAPEX
- 安価汎用サーバでNFを実現するため高額な専用機器を購入する必要がない.(CAPEX減)
- ハードウェアEOL (End of Life)
	- 用サーバなので物理的なNWハードウェアは必要ない.
	- ハードウェアアップデートも高性能なコンピュータに変えるだけなので互換がある
- 物理サーバを購入してケーブル接続する時間と比較して,
- 複数のVNFを1台のサーバーで実行できるため,冷却と電力消費が大幅に削減されます.

OPEX
- ソフトウェアによりVNFの迅速な垂直/水平スケーリングが可能 (OPEX減)
  プロビジョニング時間が大幅に短縮されます. (OPEX減)


## Frameworks and Standards

- ETSI NFV ISG
- ETSI NFV Group Specification(GS)

NFVのアーキテクチャフレームワーク

ETSI GS NFV-002は, NFの仮想化に必要なさまざまな要素を定義することによって,
NFVを標準化します.

このフレームワークは以下の二つ主要なセクションを含む
- High Level NFV Framework
- Reference Architecture Framework

### High Level NFV Framework

このフレームワークは, 物理インフラストラクチャと
仮想インフラストラクチャにおけるNFの実装を高いレベルで
記述しています.  これは, 3つのNFV作業ドメインに基づいています.

- VNF(仮想NF)<br>
  NFVI上で実行できるNFのソフトウェア実装.

- NFVi(NFV Infrastructure)<br>
	- VNFのための計算資源, ストレージ, NW接続を管理する

- NFV Management and Orchestration (MANO) <br>
  VNFの プロビジョニングに必要な管理とオーケストレーション,
  ライフサイクル管理をカバーする.

![img](img/fig01.png)

### Reference Architecture Framework

このフレームワークは, 前述のHigh Level NFV Frameworkを拡張する.
オペレータのNW内で発生する可能性のある変更にさらに焦点を当てている.
Management and Orchestrationドメイン内の新しい機能ブロックに加えて,
OSS/BSSとEMSの追加,および他のNFV作業ドメインへのリンク方法も見ることができる.

![img](img/fig02.png)

**NFV Management and Orchestration (MANO)**

MANO Group Specificationでは,
VNFのプロビジョニング方法, 構成, およびインフラストラクチャの展開を実行
するためのフレームワークが定義されている.

MANOコンポーネント<br>
- エンティティ
- 記述子ファイル
- VNFリポジトリ
- 機能ブロック

エンティティ: MANOベース環境を形成する基本的な要素<br>
- VDU (Virtual Deployment Unit): VMやコンテナを表す
- VL (Virtual Link): VNF間の接続を提供する
- CP (Connection Point): 仮想リンクの対応する接続ポイント.
- VNFFG (Virtual Forwarding Graph): VNF間の転送パス
- VNF (Virtual Network Function)
- PNF (Physical Network Function)
- NS (NW Service): VNFs, VNFFGs, VLs, CPsのセット

記述子ファイル: 所与のエンティティの構成パラメータを記述する. <br>
- NSD    : NS Discriptor
- VNFD   : VNF Discriptor
- VNFFGD : VNFFG Discriptor
- VLD    : VL Discriptor
- CPD    : CP Discriptor

リポジトリ: MANOに情報を保持する.
- NSカタログ: 使用可能なすべてのNWサービス記述子(NSD)のリポジトリ.
- VNFカタログ: すべての使用可能なVNF記述子(VNFD)のリポジトリ.
- NFVインスタンス: すべてのVNFインスタンスとNWサービスインスタンスの情報を保持します.
- NFVIリソース: エンティティを実行するために利用されるNFVIリソースのリポジトリ.

機能ブロック: NFV MANOは3つの機能ブロックで構築されている.<br>
- NFVO (NFV Orchestrator) <br>
  新しいNWサービス(NS),VNFFG,およびVNFパッケージを搭載しています.
	NFVIリソース要求を承認し, 検証します.  それに伴い, NSのライフサイクルを
	管理し, NFVI要求の検証/承認を実行します.
- VNFM (VNF Manager) <br>
  FCAPS(障害, 構成, 会計, パフォーマンスおよびセキュリティ管理)とともに,
	作成および終了などの VNF のライフサイクルを管理します.
- VIM (Virtual Infrastructure Manager) <br>
  仮想/物理コンピューティング, ストレージ, NWリソースなどの
	NFVI(NWファンクション仮想化インフラストラクチャ)を
	制御および管理します.  イベントとパフォーマンスメトリックの収集に加えて

![img](img/fig03.png)


## Modeling

OASIS TOSCA<br>
クラウドベースのWebサービス, コンポーネント, 関係,
およびそれらを管理するプロセスのトポロジを記述するための,
OASISの標準言語である, クラウドアプリケーション (TOSCA)の
トポロジとオーケストレーションの仕様 .
[5] これは, 基盤となるプラットフォームやインフラストラクチャに関係なく,
クラウドプロバイダー間で移植性と無関係な自動化管理を実現する.  [6]

つまり,TOSCAを使用すると,アプリケーション全体を1つのテンプレートにモデル化し,
それを使用してアプリケーションを第三者のインフラストラクチャ(クラウド)に
展開することができます.

コンポーネント<br>
TOSCAはアプリケーション全体をどのようにモデリングしていますか？
アプリケーション全体のモデリングはサービステンプレート(図4)から始めます.
サービステンプレートは,アプリケーションの構造(トポロジテンプレート)と
オーケストレーション(計画)を指定するために使用されます.

![img](img/fig04.png)

Node TypesとRelationship Types
- Node Types: 計算,データベース,NWなどのトポロジ内のコンポーネント
- Relationship Types: ノードが互いにどのように接続されているかを説明する

これらの両方には,プロパティとインタフェースが含まれます.
- プロパティ <br>
  Node TypeがComputeの場合,プロパティにはRAM,CPU,ディスクなどが含まれます.
- Interfaces <br>
  これは,TOSCAオーケストレーターが操作を実行するために使用する
	エントリーポイントです(「デプロイ」,「パッチ」,「スケールアウト」などの
	ライフサイクル操作など)

ビルディングブロックが完成したので,トポロジテンプレートを構築できます.
これはノードテンプレートとリレーションシップテンプレートから構築されます.
テンプレートはタイプのインスタンスです.つまり,ノードテンプレート(インスタンス)
はノードタイプ(クラス)のオカレンスです.

サービステンプレートの外には, PolicyとWorkflowsがあります.
- Policy: ノードの条件または一連のアクションを定義します.
- Workflows: 手動で定義された一連のタスクを順番に実行するように定義できます.

最後に,TOSCAは 代替マッピング と呼ばれる機能をサポートしています .
これにより,基本的にサービステンプレート全体を取り出し,
別のサービステンプレート内で再利用することができます.
たとえば,図5は,データベースの再利用による例を示しています.

![img](img/fig05.png)

TOSCA NFV<br>
再利用性に関するTOSCAsの柔軟性は,NWトポロジーの記述に理想的.
これだけでなく,ポリシー(VNF監視/スケーリング)とともにワークフロー
(VNFライフサイクルを考える)を定義する能力は,NFVに非常に適している.

さらに,TOSCAは,MANOに基づいて,NFV固有のデータモデルを指定します.
これにより,NWサービス,VNF,VNFFGなど,MANO内のさまざまな
エンティティをモデル化する機能が提供されます.

図6では,TOSCA NFV標準を使用して構築されたVNFDの例を見ることができます.
このVNFDは,1つのvCPU,512MbのRAM,1Gbのディスク,アンチスプーフィングを
無効にした単一の管理インターフェイスでVNFを構築します.

![img](img/fig06.png)


## SDN and NFV

NFVとSDN(Software Defined Networking)との違いは何ですか？  <br>

SDNは, NW制御プレーンをデータプレーンから分離する概念です.
2つをデカップリングすることで,集中モデルが可能になります.
これにより,NWのプログラミングが可能になるだけでなく,
計算と管理の集中化も可能になります.

NFVは基盤となる物理的なハードウェアからコモディティベースのサーバーにNFを
抽象化することです.

SDN/NFVはどちらもNW抽象化のレベルを実行しようとします.
ただし,その抽象化の範囲は多少異なります.

それでもなお,混乱が見られることがあります. 定義された一連のサービス機能を
使用して特定のフローでトラフィックステアリングを選択して実行する技術である
Service Function Chaining(SFC)の役割を果たすことにしましょう.
この役割を果たすために,SDNを使用して制御プレーンをプログラムし,
それに応じてフローを調整します. しかし一方では,EF MANOベースのモデル
(後述の章で説明します)内のNFVOなど,NFVオーケストレーションレイヤを
介して作成された一連の記述式を介して,一連のVNFを介してトラフィックを
誘導しています.

いずれにしても,SDN/NFVワーキンググループと現在進行中の技術のスルーに基づいて,
これらのような矛盾するトピックが確実に時々発生することを覚えておくことは
重要です. しかし,ポイントをあまりにも上げてはいけません.

NW内で抽象化しようとしているものが明確である限り,以下である
- SDNはcplane抽象化
- NFVはハードウェア抽象化

## Virtual Switches

このセクションでは, LinuxBridge, OvS, VPPについて説明する.

### Linux Bridge

2.2で最初に導入されたカーネルモジュールである. 仮装Switchingを提供する.

対応機能
- STP
- 802.1Q VLANタギング
- フィルタリング (ebtable/iptablesを介して実行)
- MAC学習L2SW

### OvS

OpenvSwitch,多層ソフトウェアベースのスイッチです.
OVSは,KVMやXEN [10]:[10] などのLinuxベースのハイパーバイザー用に設計された
機能豊富なバーチャルスイッチ製品がなかったため,オープンソースコミュニティの
ニーズを満たすことを目的としていました.

その主な機能
- 802.1Q VLANタギング
- LACP,QoS,およびさまざまなトンネリングプロトコル(GRE,VXLAN,STT,LISPなど)
- OpenFlow

OvSは事実上の標準的な仮想スイッチングオプションになります.

フロー転送の場合OVSはすべての新しいフローをユーザ空間ovs-vswitchd(slow path)
経由で転送します.
これにより,ノースバウンドコントローラはOpen vSwitchを以下のいずれかを介して設定し,
新しいフローがユーザ空間内のovs-vswitchdを通過すると,フローはカーネル内に
キャッシュされます. それ以上のフローはキャッシュに対して照合され,
OVSカーネルデータパスによって転送されます.(fast path)

- OVSDB: ポート,ブリッジ,インタフェースの作成,削除,変更など,OVS自体の設定 [11]
- OpenFlow: フローエントリをプログラムするために使用されます.

![img](img/fig07.png)

OVSには, NormalとFlowという 2つの動作モードがある.
- Normal<br>
  通常モードでは,各OVSは通常のレイヤ2ラーニングスイッチとして機能します.
	つまり,送信元MACアドレスは入力ポートに対して受信フレームごとに学習されます.
	フレームは,MACアドレスに基づいて適切なポートに転送されるか,
	またはMACアドレスがまだ学習されていない場合,またはマルチキャストまたは
	ブロードキャストフレームである場合は,すべてのポートにフラッディングされます.
- Flow <br>
  転送の決定は,OpenFlowプロトコルの使用によるフローを使用して行われます.
	フローは,ノースバウンドのAPI,つまりコントローラ(コントロールプレーン)
	から受信され,Open vSwitchesデータプレーンに実装されます.
	OpenFlowの詳細については, こちらをご覧ください .

### VPP

VPP(Vector Packet Processing)はもともとシスコが作成したもので,
現在はFD.ioプロジェクトの一部です. これは高速ルーティング/スイッチングを
提供するDPDKの上で動作するユーザスペースアプリケーションです.
FD.ioは次のように定義します.

> VPPプラットフォームは,すぐに使用できる品質のスイッチ/ルータ機能を提供する
> 拡張可能なフレームワークです. シスコのVector Packet Processing(VPP)
> テクノロジのオープンソースバージョンです.汎用CPUで実行できる高性能の
> パケット処理スタックです.
> VPPのこの実装の利点は,高性能で実証済みのテクノロジー,そのモジュール性と
> 柔軟性,豊富な機能セットです. [12]

VXLAN,IPSEC,MPLS,VRF,ECMP,およびL2 ACL(ただしこれに限定されない)
などの幅広いL2 / L3テクノロジをサポートしています.

VPPはユーザー空間プロセスとして実行され,DPDK(IO Optimizationセクションに
含まれています)と統合され,割り込みなしのデータパスをカーネル空間外に提供します.

VPPは,パケット処理グラフ(図8)を介してパケットをベクトル処理することによって動作する.
グラフ全体を通して単一のパケットを処理する代わりに,パケットのベクトルは
最初に単一のグラフノードを通して処理される.
完了すると,次のグラフノードを介して処理されます.

![img](img/fig08.png)

ベクトルの最初のパケットがキャッシュをスピンアップします.
さらなるパケットは非常に迅速に処理されます. VPPの1つの類推は,

> 木材の各部分が切断され,研磨され,孔が穿孔される必要がある木材のスタックの
> 問題を考えてみましょう. 一度に1つのツールしか手に入れることはできません
> (命令キャッシュに似ています). 最初に鋸を拾い,すべての切断を行い,
> その後にサンダーを拾い上げてすべてのサンディングを行い,ドリルを拾い上げて
> すべてを行うならば,裁断,サンディング,および掘削をより速く完了することに
> なりますあなたの掘削. 木材1枚ごとに各工具をピックアップするのがはるかに
> 遅くなります. [14]

VPPグラフが提供するパフォーマンス上の利点に加えて,新しいプラグインと
拡張機能を導入する能力も非常に柔軟です.


## Optimization

NFVに関して重要な懸念事項の1つは,パフォーマンスに関するもの.
このセクションでは,VNFのパフォーマンスを大幅に向上させるために使用できる,
業界内で利用可能なさまざまな最適化手法と技術について説明する.

### System Optimization

**NUMA**<br>
従来,SMP(Symmetric Multiprocessor)マシンでは,システム内のすべての
CPUが同じメモリにアクセスします. しかし,CPUの技術が進化し,CPUの
性能が向上するにつれて,メモリバスへの負荷と,CPUがメモリからデータが
到着するのを待つ時間が増加しました.

NUMA(Non-Uniform Memory Access)は,メモリを複数のメモリノード(またはセル)
に分割することでこれを処理します.複数のメモリノード(またはセル)は,
1つまたは複数のCPUに対して「ローカル」です. [15]

NUMAノードは相互接続を介して接続されているため,すべてのCPUがすべてのメモリに
アクセスできます. インターコネクトのメモリ帯域幅は通常,個々のノードのメモリ
帯域幅よりも高速ですが,多くのノードからの同時クロスノードトラフィックに
よっても圧倒される可能性があります. [16] したがって,NUMAは
ローカルメモリノードにアクセスするCPUのメモリアクセスを高速化しますが,
(インターコネクト経由の)リモートメモリノードへのアクセスは遅くなります.
ここではNUMAトポロジーの認識が必要です.

![img](img/fig09.png)

**CPU Pinning**

ハイパーバイザーがゲストのvCPUを仮想化する場合は,通常,システム内のどのコア
でもスケジュールされます. もちろん,この動作は,NUMAノード内のCPUコア間または
NUMAノード間のCPUコア間で仮想CPUがスケジューリングされるため,最適な
キャッシュパフォーマンスにつながる可能性があります. [18] CPUのピン割り当ては,
仮想CPUがどの物理CPUを実行するかを制限する機能を提供します.その結果,
CPUからのメモリ読み込みと書き込みが高速になります.

**Huge Paging**

システム内では,プロセスは仮想メモリ(ページとも呼ばれます)で動作します.
仮想メモリは物理メモリにマッピングされ,マッピングはページテーブルと呼ばれる
データ構造内に格納されます.

プロセスがメモリにアクセスするたびに,カーネルは このページテーブルに [19]
を 書き込むことによって,望ましい仮想メモリアドレスを物理アドレスに変換ます.
しかし,ページテーブルは非常に複雑で低速で,一部のプロセスがメモリにアクセス
するたびにデータ構造全体を解析することはできません. [20] ありがたいことに,
CPUにはTLB(Translation Lookaside Buffer)と呼ばれるハードウェアキャッシュが
含まれており,物理から仮想へのページマッピングがキャッシュされ,ルックアップ
が高速化されます. ただし,TLBの容量は限られています.
これは巨大なページが入る場所です.ページのサイズを大きくすると,TLB内で
より多くのメモリをマッピングし,キャッシュミスを減らし,メモリアクセス時間を
短縮することができます.

```
                   Pagesize     TLB-Entry      Map-Size     Ex
----------------------------------------------------------------------------
Without Hugepages  4096B        512            2MB          4096B*512=2MB
With Hugepages     2MB          512            1GB          2MB*512=1GB
```

### I/O Optimiazation

まず,さまざまなパケット最適化手法について説明します.まず,パフォーマンスの
低下を引き起こす可能性のある問題のいくつかを見てみましょう.VNFが今日の
NWで要求される低遅延10Gbの速度に達するのを防ぎます.

図10に基づいて,仮想インスタンスとともに仮想スイッチを備えたシステムがあります.
ハイパーバイザーとLinuxブリッジ(つまり仮想スイッチ)は,カーネルモードで
実行され,仮想マシンはユーザーモードで実行されます.

より詳細にフローを見てみましょう.

NIC to Linux Bridge - フレームがNICによって受信されます. それは,
DMA(ダイレクトメモリアクセス)を介して,事前に割り当てられた
RXシステムメモリバッファに直接移動される. NICはCPUに割り込み要求を発行し,
フレームがRXメモリバッファからカーネルスタックに移動することを要求する.
これは,カーネルドライバによって実行されます.

残念なことにハードウェアの割り込み(CPU使用量の点で)は高価です.
プロセッサは,現在実行している処理を中断し,その状態を保存し,
次にイベントを処理する割り込みハンドラを初期化することをプロセッサに
要求する優先順位の高い状態をCPUに警告する.

フレームがカーネル内に入ると,フレームはNW・スタックを介して
Linux Bridgeまで処理されます. しかし,カーネルNWスタック
は本質的に遅いです. ARPからVXLANまで,さまざまな機能やプロトコルを
処理しなければならず,すべてを安全に行う必要があります. [21]

Linuxブリッジから仮想インスタンスへのブリッジ - フレームがLinuxブリッジに
よって受信され,パケットが仮想マシン向けであることを判別するために必要な
操作が実行されると,カーネルはシステムコールを実行します.

システムコールはコンテキストスイッチを開始します. これにより,
CPUが異なるアドレス空間で動作するように切り替わります. これにより,
ユーザー空間内で実行されている仮想インスタンスがカーネル空間に
「コンテキスト切り替え」し,フレームをさらに処理できるようにユーザー空間に
戻る前にフレームを取得できます. もちろん,これは非常に効率的ではありません.
パックを取得するには,実際のデータ(/フレーム)のコピーを実行するために
必要なコスト/オーバーヘッドはもちろん,仮想インスタンスは各IO要求に対して
何をしているのかを停止する必要があります2つの異なるアドレス空間

![img](img/fig10.png)

このフローは大幅に簡素化されていますが ,フレームをカーネルデータパスと
システムメモリ空間の間で移動させることは計算コストが高いことを強調しています.

**SR-IOV**

SR-IOV(シングルルートI / O仮想化) は,PCI Express(PCIe)仕様の拡張版です.
VMごとに個別の物理NICのように動作するVirtual Functions(VF)が作成されます.
NICのVFには,それがサービスする特定のVMによって所有されているユーザ空間メモリが
どこにあるかを示すディスクリプタが与えられます.
これにより,NICとの間で送受信されるデータを,DMA(ダイレクトメモリアクセス)
経由で直接VMとの間で転送し,仮想スイッチをバイパスし,割り込みのない動作を
提供し,高速パケット処理を実現します. しかし,欠点があります. VMとVFとの
間の直接的なアクセスにより,仮想スイッチ間の何かがバイパスされる.

仮想関数に加えて,SR-IOVは物理関数(PF)も提供します. 物理機能は,
他のPCIeデバイスと同様に検出,管理,操作が可能な完全機能のPCIe機能です.
これらは,SR-IOV機能構造を含み,SR-IOV機能(仮想機能の設定や構成など)
を管理します.

![](img/fig11.png)

**DPDK**

DPDK(データプレーン開発キット)   高速パケット処理のためのさまざまな
ユーザ空間ライブラリとドライバからなるフレームワークです. [23]
もともと,x86ベースのCPU上で動作するようにIntelによって開発されたDPDKは,
IBM POWERやARMなどの他のCPUタイプをサポートしています.

DPDKは,パケットスループットを最適化するためにいくつかの手法を使用しますが,
ファーストパス と PMDに 基づいています(そのパフォーマンスの鍵となります) .

ファストパス(カーネルバイパス)   - ファストパスは,NICからカーネルを
バイパスしてユーザー空間内のアプリケーションに作成されます.
これにより,ユーザ空間/カーネル空間の間でフレームを移動するときの
コンテキスト切り替えが不要になります.
さらに,カーネルスタック/NWドライバを否定することで,
さらに利益を得ることができます.

ポールモードドライバ - フレームが受信されたときにNICがCPUに割り込みを
発生させる代わりに,CPUはポーリングモードドライバ(PMD)を実行してNICに
常に新しいパケットをポーリングします.
ただし,これは,CPUコアをPMDを実行するために専用に割り当てて割り当てる必
要があることを意味します.
さらに,DPDKフレームワークには次のものが含まれています.

バッファー・マネージャー は,NW・バッファー・キューの
割り振り/割り振り解除を最適化します.
メモリノード間の高価なメモリ操作を回避する NUMAの 意識.
CPUのTLB内の物理 - 仮想ページのマッピングを最適化するための Hugepages

![img](img/fig12.png)

DPDK対応のCPUだけでなくDPDK対応のNICも必要(大抵のIntelNICなら大丈夫)

**OVS-DPDK**

ボックスの外でOVSはカーネルベースのデータパスと,ユーザスペースデーモン
ovs-vswitchd(図13)を実装しています. このデーモンは新しいフローを処理し,
北向きのコントローラがプログラムでコントロールプレーンを更新できるようにします.
しかしながら,これは,フレームがカーネル内にキャッシュされているにもかかわらず,
仮想関数による処理のためにユーザ空間に移動されなければならないという問題が生じる.
既に見てきたように,フレームをアドレス空間の間で切り替えるコンテキストは,
CPUサイクルに関して高価です.

![img](img/fig13.png)

Open vSwitch(OVS)は,OVS 2.2でDPDKのサポートを導入しました.
OVS-DPDKは,OVSカーネルのデータパスをユーザー空間に移動します.
ポールモードドライバ(PMD)を使用してNICから直接フレームを取得し,
カーネルをバイパスすることができます.
フレームとVNFの両方がユーザー空間内にあるので,vHostユーザーなど,
OVSとVNFの間でフレームを移動するための技術をさらに使用できます.

vHostユーザー はOVS 2.4.0で導入されたドライバで,QEMU(仮想化ソフトウェア)
がゲストの仮想キューのメモリ位置をDPDKと共有できるようにします.
QEMUは,OVSDPDK内のドライバのバックエンド実装を介してメモリ割り当てを共有しする.
これにより,OVS-DPDKはDMAを介してキュー間でフレームを送受信でき,
パフォーマンスが大幅に向上します.

### Crypto Optimization

**AES-NI**

もともとIntelによって開始された
AES-NI(Advanced Encryption Standard New Instructions)は,
IntelおよびAMDベースのCPU用のx86命令セットの拡張版です.
命令セットの目的は,暗号化および復号化を実行するアプリケーションの速度を
向上させることです.   高度な暗号化規格 (AES). [24]
これにより,SSL終了を実行するロードバランサやIPSEC VPNゲートウェイなどの
暗号ベースのVNFを高速化できます.

**QUICKASSIST**

Intel QuickAssistは,暗号化や圧縮などの計算集約型機能の高速化技術です.
使用例の例は,

対称暗号   暗号操作と認証操作を含む機能
RSA,Diffie-Hellman,楕円曲線暗号などの 公開鍵機能
圧縮および解凍   機能. [25]
QuickAssistは,27xx8および3xx8 Atom CPUに統合されています.
また,インテル®8925〜8955シリーズのインテル®チップセットにも組み込まれており,
インテル®ベースのCPUからの負荷を軽減できます.

APIを使用すると,QuickAssistはアプリケーションに公開されます(図14).
APIの役割は,Acceleration Driver Framework(ADF)およびService Access Layer(SAL)と
対話し,Acceleration Engineのファームウェアがアクセラレーションチップに存在することです.


![img](img/fig14.png)

## Projects

標準とフレームワークは素晴らしいですが,実際に使用する方法がない場合,
それは何ですか？

このセクションでは,MANOとフルスタックプロジェクトの両方を見て,
業界内の主要なオープンソースプロジェクトを見ていきます.
MANOは,NFVO,VNFM,VIMの役割の一部または全部をカバーしています.
データパス技術(DPDKなど),仮想スイッチング技術(OVS,VPPなど),
SDNコントローラなど,スタック全体を含むフルスタック

### ETSI MANO

**オープンソースMANO(OSM)**

オープンソースのMANO は オープンソースのNFV MANOソフトウェアスタックを提供する
n個の ETSIホストプロジェクトです. OSMは,ETSIのIndustry Specification Group
(ISG)と連携しています. OSMの主な機能には ,次の ものが あります.

強化されたプラットフォーム認識(EPA) により,OpenStackはVMを最も適切な
プラットフォーム上で実行し,システムに組み込まれた機能から追加の利点を
得ることができます. [26]
SDNアンダーレイ制御 は,必要に応じて各リンクの帯域幅を保証します.
マルチVIM機能 - VMwareやOpenStackなどの複数の仮想インフラストラクチャ管理を
サポートします.
マルチテナントVNF -
マルチテナントVNF とシングルテナントVNFの導入と設定が可能です.

**オープンバトン**

オープンバトン は,ETSI NFV準拠のMANOフレームワークです.
Open Batonは,NFVOやVNFMなどの一般的なMANO機能の上に, 次の機能を提供します.

VNFのスケーリングを 可能にする 自動スケーリングエンジン .
障害の 自動管理に 使用される 障害管理システム .
オーケストレーションロジックに何かを書き直すことなく,
異なる VIM を追加および削除するための VIMプラグイン .
Zabbix Zabbix監視システムとの統合.
VNFM 独自のVNFMを構築できる ライブラリ .

**OpneStack Tracker**

Tacker は,VNFDのカタログ,VNFのプロビジョニング,VNFの設定管理,
VNFの監視と自動修復を提供するMANO Architectural Frameworkを
ベースにした公式のOpenStackプロジェクトです. Tackerは 以下を 提供 します：

TOSCAモデリング - TOSCA NFVテンプレートを使用してVNFを記述 し,
Tacker VNFカタログに搭載されています. VNFは,TOSCAテンプレートを
OpenStack Heatに変換することによって開始されます.
VNFモニタリング   - ICMPまたはHTTP pingを介して実行されます.
VNFが応答しない場合 , Tackerはインスタンスを再構築してAuto Healを行います.
VNF構成管理
VNF成をプッシュできるようにベンダが開発できる構成ドライバを介して実行されます.

### Full Stack

**ONAP**

ONAP( Open Network Automation Platform )は,AT＆TのECOMPと
China Mobile / Telecoms Open-Oプラットフォームの統合に基づいて構築されています.
ONAPは,

> ...設計,作成,オーケストレーション,監視,およびライフサイクル管理のための
>   機能を提供するオープンソースソフトウェアプラットフォームは,
>
> - 仮想NF(VNF)
> - それを含むキャリアスケールのソフトウェア定義NW(SDN)
> - 上記の[27]を組み合わせた上位レベルのサービス

MANOの標準コンポーネント(VNFM,NFVOなど)は,
さまざまな豊富な機能を提供するために,いくつかの個別のONAPサービスと
コンポーネントに組み込まれています.
ONAPはOpen Baton,Tacker,OSMに見られるすべての機能を提供しますが,
以下のコンポーネント(ただしこれらに限定されません)
を含む幅広い追加機能も提供しています：

- アクティブかつ利用可能なインベントリ(AAI)
  利用可能なリソースとサービス,およびそれらの関係をリアルタイムで表示します.[28]
- マスターサービスオーケスト レーター (Master Service Orchestrator,MSO)
  管理された環境内の論理リソースと物理リソースの作成,変更,または削除を
	調整するためのルールとポリシーに基づいて,タスクをアレンジ,シーケンス,
	および実装します. [29]
- セキュリティフレームワーク
  ソースコードのセキュリティ分析,脆弱性のスキャン,脆弱性の修正,
	VNFからの分析およびNWデータに基づくポリシーの実行を可能にします.
- VNF の必要なワークフロー(ライフサイクル管理,アクティベーションなど)の
  定義,カタログ化,構築を可能にする SDC( Service Design and Creation)
	モデリング/設計ツール.

![img](img/fig15.png)

**OPNFV**

OPNFV( Open Platform NFV )は,OpenDaylight,ONOS,OpenStack,Ceph,KVM,
Open vSwitch,Linuxなどの既存のオープンソースプロジェクトを統合してNFVの
パッケージ化されたオープンソースプラットフォームを提供することを目的とした,
インフラストラクチャ(NFVI)および仮想化インフラストラクチャ管理(VIM)

OPNFVは他のオープン・ソース・コミュニティと協力して,
作業から得た貢献と学習の両方を,青写真,パッチ,および新しいコードの形で
それらのコミュニティに直接もたらします. [30]
アップストリーム寄稿に加えて,OPNFVには,OPNFV のさまざまなコンポーネントを
活用するDoctor(VNF自己修復)やMoon(セキュリティ管理)など ,
数多くのプロジェクトが あります.

![img](img/fig16.png)


## References

[1 ][1]   "ETSI - NFV." Accessed 22 Aug. 2017. <br>
[2 ][2]   "GS NFV 002 - V1.1.1 - Network Functions Virtualisation <br>
[3 ][3]   "GS NFV 002 - V1.1.1 - Network Functions Virtualisation <br>
[4 ][4]   "The Definition of OSS and BSS | OSS Line." 5 Dec. 2010, Accessed 22 Aug. 2017. <br>
[5 ][5]   "OASIS TOSCA - Wikipedia."  Accessed 21 Aug. 2017. <br>
[6 ][6]   "TOSCA - Oasis." Accessed 22 Aug. 2017. <br>
[7 ][7]   "Topology and Orchestration Specification for Cloud ... - Name - Oasis. Accessed 22 Aug. 2017. <br>
[8 ][8]   "TOSCA Simple Profile in YAML Version 1.0 - Name - Oasis.".  Accessed 24 Aug. 2017. <br>
[9 ][9]   "tacker/samples/tosca-templates/vnfd at master · openstack ... - GitHub". Accessed 24 Aug. 2017. <br>
[10][10]  "TripleO, NUMA and vCPU Pinning: Improving Guest ... - StackHPC Ltd." 3 Feb. 2017,  Accessed 24 Aug. 2017. <br>
[11][11]  "What is Open vSwitch Database or OVSDB? - Definition - SDxCentral."  Accessed 21 Aug. 2017. <br>
[12][12]  "VPP/What is VPP? - fd.io Developer Wiki." 26 May. 2017,  Accessed 21 Aug. 2017. <br>
[13][13]  "Technology – The Fast Data Project - FD.io." Accessed 24 Aug. 2017. <br>
[14][14]  "Technology – The Fast Data Project - FD.io." Accessed 21 Aug. 2017. <br>
[15][15]  "CPU Pinning and NUMA Awareness in OpenStack - Stratoscale." 15 Jun. 2016, Accessed 22 Aug. 2017. <br>
[16][16]  "Driving in the Fast Lane – CPU Pinning and NUMA Topology ...." 5 May. 2015, Accessed 24 Aug. 2017. <br>
[17][17]  "OpenStack in Production: NUMA and CPU Pinning in High ...." 3 Aug. 2015, Accessed 21 Aug. 2017. <br>
[18][18]  "TripleO, NUMA and vCPU Pinning: Improving Guest ... - StackHPC Ltd." 3 Feb. 2017,  Accessed 24 Aug. 2017. <br>
[19][19]  "Mirantis OpenStack 7.0 NFVI Deployment Guide: Huge pages." 25 Jan. 2016,  Accessed 24 Aug. 2017. <br>
[20][20]  "Advantages and disadvantages of hugepages – TechOverflow." 18 Feb. 2017, Accessed 24 Aug. 2017. <br>
[21][21]  "Pushing the Limits of Kernel Networking - Red Hat Enterprise Linux Blog." 29 Sep. 2015,  Accessed 22 Aug. 2017. <br>
[22][22]  "Using PCIe SR-IOV Virtual Functions - Oracle VM Server for SPARC ...." Accessed 21 Aug. 2017. <br>
[23][23]  "DPDK.org." http://dpdk.org/. Accessed 21 Aug. 2017. <br>
[24][24]  "AES instruction set - Wikipedia.". Accessed 21 Aug. 2017. <br>
[25][25]  "Intel Quickassist » ADMIN Magazine." . Accessed 21 Aug. 2017. <br>
[26][26]  "Openstack Enhanced Platform Awareness - Intel Open Source ...." . Accessed 24 Aug. 2017. <br>
[27][27]  "What is ONAP? - Developer Wiki - Confluence." 18 Jul. 2017, Accessed 25 Aug. 2017. <br>
[28][28]  "Active and Available Inventory (AAI) - Developer Wiki - ONAP." 13 Apr. 2017,. Accessed 25 Aug. 2017. <br>
[29][29]  "Master Service Orchestrator (MSO) - Developer Wiki - ONAP." 13 Apr. 2017, . Accessed 25 Aug. 2017. <br>
[30][30]  "Learn the Mission - Mission – OPNFV.". Accessed 22 Aug. 2017. <br>
[31][31]  "Enabling the Transition: Introducing OPNFV, an integral step towards ....". Accessed 21 Aug. 2017. <br>


[1]:http://www.etsi.org/technologies-clusters/technologies/nfv
[2]:http://www.etsi.org/deliver/etsi_gs/nfv/001_099/002/01.01.01_60/gs_nfv002v010101p.pdf
[3]:http://www.etsi.org/deliver/etsi_gs/nfv/001_099/002/01.01.01_60/gs_nfv002v010101p.pdf
[4]:http://www.ossline.com/2010/12/definition-oss-bss.html
[5]:https://en.wikipedia.org/wiki/OASIS_TOSCA
[6]:https://www.oasis-open.org/committees/tosca/faq.php
[7]:http://docs.oasis-open.org/tosca/TOSCA/v1.0/os/TOSCA-v1.0-os.html
[8]:http://docs.oasis-open.org/tosca/TOSCA-Simple-Profile-YAML/v1.0/csprd01/TOSCA-Simple-Profile-YAML-v1.0-csprd01.html
[9]:https://github.com/openstack/tacker/tree/master/samples/tosca-templates/vnfd
[10]:https://www.stackhpc.com/tripleo-numa-vcpu-pinning.html
[11]:https://www.sdxcentral.com/cloud/open-source/definitions/what-is-ovsdb/
[12]:https://wiki.fd.io/view/VPP/What_is_VPP%3F
[13]:https://fd.io/technology/
[14]:https://fd.io/technology/
[15]:https://www.stratoscale.com/blog/openstack/cpu-pinning-and-numa-awareness/
[16]:http://redhatstackblog.redhat.com/2015/05/05/cpu-pinning-and-numa-topology-awareness-in-openstack-compute/
[17]:http://openstack-in-production.blogspot.co.uk/2015/08/numa-and-cpu-pinning-in-high-throughput.html
[18]:https://www.stackhpc.com/tripleo-numa-vcpu-pinning.html
[19]:https://www.mirantis.com/blog/mirantis-openstack-7-0-nfvi-deployment-guide-huge-pages/
[20]:https://techoverflow.net/2017/02/18/advantages-and-disadvantages-of-hugepages/
[21]:http://rhelblog.redhat.com/2015/09/29/pushing-the-limits-of-kernel-networking/
[22]:https://docs.oracle.com/cd/E35434_01/html/E23807/usingsriov.html
[23]:http://dpdk.org
[24]:https://en.wikipedia.org/wiki/AES_instruction_set
[25]:http://www.admin-magazine.com/Archive/2016/33/Boosting-Performance-with-Intel-s-QuickAssist-Technology
[26]:https://01.org/sites/default/files/page/openstack-epa_wp_fin.pdf
[27]:https://wiki.onap.org/pages/viewpage.action?pageId=1015843
[28]:https://wiki.onap.org/pages/viewpage.action?pageId=1015836
[29]:https://wiki.onap.org/pages/viewpage.action?pageId=1015834
[30]:https://www.opnfv.org/about/mission
[31]:https://www.opnfv.org/blog/2014/09/30/enabling-the-transition-introducing-opnfv-an-integral-step-towards-nfv-adoption




