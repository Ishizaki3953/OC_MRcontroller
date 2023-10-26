#### Readme

##### 変更履歴
【OC_MRcontroller_ver1.1】
・設定ボタンでASW/ASDを同時起動する仕様に変更

【OC_MRcontroller_ver1.0】
・「OC_MRWheel_ver2.1」をベースに「OC_MRDial_ver3.1」のデモ動作関数をマージした。
・Demo.cpp→「Demo_asd.cpp」「Demo_asw.cpp」のように変更

##### 不具合情報
・「#define PLOT_ASW」および「#define PLOT_ASD」をアクティベートすると、ボタン同時押しの際にもう片方のASD(ASW)からパルスが出なくなる。
→plot関数を実施したときに発生している模様。原因は不明。
