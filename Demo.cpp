/**
  ******************************************************************************
  * @file           : Demo.cpp
  * @brief          : demo mode
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "Demo.h"
#include "common.h"
#include "config.h"

#define INTERRUPT_CLEAR()  \
        __disable_irq();\
        g_lclkCnt = 0;\
        g_rclkCnt = 0;\
        g_lrlsCnt = 0;\
        g_rrlsCnt = 0;\
        __enable_irq();

/**
  * @brief constructor
  * @param  Motor:motor  sensor:encoder  plot:plotter
  * @retval none
  */
Demo::Demo(Motor *motor, ISensor *sensor, Plotter *plot, WheelSignal *wheel):
    _que(sizeof(uint16_t), ZERO_TORQUE){

    _motor = motor;
    _sensor = sensor;
    _plot = plot;
    _wheel = wheel;

    _pattern = PAT_A;
    _w = 100.0f;//周期初期値100%。変動する
    _h = 100.0f;//電圧固定100%
    
    _sct = 0;
}
/**
  * @brief setting load
  * @param  none
  * @retval none
  */
void Demo::setting_load(){
    //フラッシュからデータを読み込む
    Table table;
    int8_t vals[PAT_MAX];
    table.begin_read();//読み込み開始
    int j;
    for(int i = 0; i < POSI_MAX; i++){//1024
        table.read((uint8_t*)vals, PAT_MAX);
        _pwm[i][0] = vals[0];
        _pwm[i][1] = vals[1];
        _pwm[i][2] = vals[2];
        _pwm[i][3] = vals[3];
              
//        //パターンA
//       if(i<=512) signal_pat_a[i] = 0;
//        else signal_pat_a[i] = 1;
//        //パターンB
//        if(i<=256) signal_pat_b[i] = 0;
//        else if (i<=768) signal_pat_b[i] = 1;
//        else signal_pat_b[i] = 0;       

    }
    table.end_read();//読み込み終了
}
/**
  * @brief get width (周期)
  * @param  none
  * @retval width
  */
float Demo::get_w(){
    // ad
    //       ad   w          y
    //[  8%] 100/(  8/100)=> ad=1250%1024=[226]
    //[ 10%] 100/( 10/100)=> ad=1000%1024=[1000]
    //[ 50%] 100/( 50/100)=> ad=200%1024=[200]
    //[ 90%] 100/( 90/100)=> ad=111%1024=[111]
    //[100%] 100/(100/100)=> ad=100%1024=[100]

    //              *               *
    //      0      256     512     768     1024
    //      +-------+-------+-------+-------+
    //        SEC_1   SEC_2   SEC_3   SEC_4        
    //
    // 正転の場合
    // 0->1->2->3->4     OK
    // 0->2->3->4->1->2  OK
    // 0->3->4->1->2     OK
    // 0->4->1->2->3->4  OK
    // 逆転の場合
    // 0->1->4->3->2->1  OK
    // 0->2->1->4->3     OK
    // 0->3->2->1->4->3  OK
    // 0->4->3->2->1     OK
    //
    enum Section {//区画の定義
        NONE=0, SEC_1, SEC_2, SEC_3, SEC_4,
    };
    static int i = 9; //index: default=100%
    static int cnt = 0;

    //現在区画の確認
    uint16_t x = _raw2;
    if(0 <= x && x < 256) _now_sct = SEC_1;
    else if(256 <= x && x < 512) _now_sct = SEC_2;
    else if(512 <= x && x < 768) _now_sct = SEC_3;
    else if(768 <= x && x < 1024) _now_sct = SEC_4;

    switch(_sct){
    case 0:
        _sct = _now_sct; //現在区画を覚えて次を待つ
        _plus = false;
        break;
    case SEC_1:
        if(_now_sct == SEC_2){
//            //正転またがり処理
//            if(++cnt >= 2){//2ポイントまたがった(正転)
//                if(++i >= 10) i = 9; //周期変更
//                cnt = 0;
//            }
            _plus = true;
        } else if(_now_sct == SEC_4) _plus = false;
        _sct = _now_sct;
        break;
    case SEC_2:
        if(_now_sct == SEC_1){
//            //逆転またがり処理
//            if(--cnt <= -2){//２ポイントまたがった（逆転）
//                if(--i < 0) i = 0; //周期変更
//                cnt = 0;
//            }
            _plus = false;
        } else if(_now_sct == SEC_1) _plus = false;
        _sct = _now_sct;
        break;
    case SEC_3:
        if(_now_sct == SEC_4){
//            //正転またがり処理
//            if(++cnt >= 2){//2ポイントまたがった(正転)
//                if(++i >= 10) i = 9; //周期変更
//                cnt = 0;
//            }
            _plus = true;
        } else if(_now_sct == SEC_2) _plus = false;
        _sct = _now_sct;
        break;
    case SEC_4:
        if(_now_sct == SEC_3){
//            //逆転またがり処理
//            if(--cnt <= -2){//２ポイントまたがった（逆転）
//                if(--i < 0) i = 0; //周期変更
//                cnt = 0;
//            }
            _plus = false;
        } else if(_now_sct == SEC_3) _plus = false;
        _sct = _now_sct;
        break;
    }
    
    //ゼロクロスチェック
    if(_motor->ZERO >= 2){
        _motor->ZERO = 0;
        if(_plus){
            if(++i >= 10) i = 9; //周期変更
        }else{
            if(--i < 0) i = 0; //周期変更
        }
    }
    
    return g_width[i]; //周期決定
}
/**
  * @brief ad convert
  * @param  ad
  * @retval convert ad
  */
uint16_t Demo::ad_conv(uint16_t ad, float w){//ad:0-4095
    float x = ad / (w / 100.0f);
    int ad2 = (int)x % 1024;
    return (uint16_t)ad2;
}
/**
  * @brief tick interrupt (1msec)
  * @param  none
  * @retval none
  */
void Demo::tick_interrupt(void){
    _tickCnt++;
}
/**
  * @brief 設定データのプロット
  * @param  none
  * @retval none
  */
void Demo::setting_out(){
    for(uint16_t raw = 0; raw < 500; raw++){//500plot
        float pwm = calc_pwm(raw);
        _plot->plot(pwm);
    }
}

/**
  * @brief ハードウェアボタンのチェック
  * @param  change is 強制的に設定データ更新
  * @retval true: 設定データ更新
  */
bool Demo::ChangeCheck(bool change){
    static int mode = 0;
    
    if(mode == 0){//設定モードでない
        if(g_rrlsCnt) {
            __disable_irq();
            g_rrlsCnt = 0;
            g_rclkCnt = 0;//連続右クリックをクリア
            __enable_irq();
        }
    }
    else if(mode == 1){//周期設定中
        if(g_rrlsCnt){//周期設定モードかつ右リリース
            mode = 0;
            INTERRUPT_CLEAR();
            return false;
        }
    }
    else if(mode == 2){//波形設定中
        if(g_rrlsCnt && g_lrlsCnt){//波形設定モードかつ左右リリース
            mode = 0;
            INTERRUPT_CLEAR();
        }
        return false;
    }

    if(g_lclkCnt){//ワンショット
        if(g_rclkCnt){//ワンショット
            if(g_lclick.read() == 0 && g_rclick.read() == 0){

                //左と右クリック同時押しでパターン切り替え
                g_green = 1;
                change = true;
                mode = 2;
                //パターン切り替え
                if(_pattern == PAT_D) _pattern = PAT_A;
                else if(_pattern == PAT_A) _pattern = PAT_B;
                else if(_pattern == PAT_B) _pattern = PAT_C;
                else if(_pattern == PAT_C) _pattern = PAT_D;
            }
            INTERRUPT_CLEAR();
        }
    } else {
        if(g_rclkCnt >= RLONG_TIME) mode = 1;//連続ショット
        if(mode == 1 && g_rclkCnt){//周期設定モード
            //右クリック押しながらホイール回転で周期を変える
            g_green = 1;
            change = true;
            INTERRUPT_CLEAR();
        }
    }
    
    if(mode == 1){//周期設定モード
        float w = get_w();//ホイール回転による周期更新。常時実行
        _w = w;
    }
    
    if(change){
        char buf1[8], buf2[8], buf3[8], buf4[8], buf5[8];
        //ラベルプロット
        sprintf(buf1, "P%d", _pattern+1);
        sprintf(buf2, "W%d", (int)_w);
        sprintf(buf3, "H%d", (int)_h);
        sprintf(buf4, "s%d", (int)_plus);
        sprintf(buf5, "z%d", (int)_motor->ZERO);
        _plot->label(buf1, buf2, buf3, buf4);
        
        //設定データのプロット（グラフ） ぐるぐる回している最中はやらない
        if(mode == 2){//波形設定モード
            setting_out();
        }
        g_green = 0;
    }
    return change;
}
/**
  * @brief RAWからPWMへ変換
  * @param  raw(AD) <4096
  * @retval pwm(-100 +100)
  */
float Demo::calc_pwm(uint16_t raw){//raw < 4096 
    _raw2 = ad_conv(raw, _w);//YOKO STRETCH
    int pwm = _pwm[_raw2][_pattern];//raw2 < 1024
    
    float pwm2 = pwm * _h;
    float pwm3 = pwm2 / 127.0f;
    
    //printf("[AD]%d->%d, [VAL]%d->%d%%, w=%f, h=%f\n", raw, _raw2, pwm, (int)pwm3, _w, _h);
    return pwm3;
}
/**
  * @brief デモ開始
  * @param  none
  * @retval none
  */
void Demo::start(){//setting -> demo
    setting_load();
    _tickCnt = 0;
    _tick.attach(this, &Demo::tick_interrupt, 0.001);//1ms
    if(ChangeCheck(true)){//true: 強制的に設定データの初期化
        _step = 1;//設定データプロット出力へ
    }
}
/**
  * @brief ゼロトルク期間チェックと同時にPWMを返す
  * @param  none
  * @retval pwm
  */
float Demo::ZeroTorqueCheck(){
           
    _que.Put(&_raw);//RAWをストック(max500)
    
    float pwm = calc_pwm(_raw);//RAWからPWMへ
    
    if(_que.GetNumberOfItems() >= ZERO_TORQUE){//RAWが500個たまった場合
        static uint16_t lastRaw[ZERO_TORQUE];//ビッグサイズのため静的配置
        int cnt = 0;
        const int N = ZERO_TORQUE;//check point
        
        _que.PeekAll(lastRaw);//すべてのストックRAWを取得
        
        //500個の変動状況を調べる
        for(int i = 0; i < N; i++){
            int min = (_raw > 0)? _raw - 1 : -1;
            int max = _raw + 1;
            if(min <= lastRaw[i] && lastRaw[i] <= max){//変動許容範囲であれば
                cnt++;//変動なしとしてカウントアップ
            }
        }
        _que.Get(lastRaw);//最古RAWを削除
        
        //ゼロトルク期間のチェック
        static int wait = -1;
        if((wait == ZERO_TORQUE || wait < 0) && cnt == N){
            pwm = 0;//ゼロトルクとする
            wait = ZERO_TORQUE;//[ms]
        } else {
            wait--;
        }
    }
    return pwm;
}

///**
//  * @brief calc_signal_a
//  * @param  raw(AD)
//  * @retval signal_a
//  */
//bool Demo::calc_signal_a(uint16_t raw){//raw < 4096
//    int _raw_calc_signal_a = ad_conv(raw);//YOKO STRETCH
//    return signal_pat_a[_raw_calc_signal_a];//_raw_calc_signal_a < 1024
//}
///**
//  * @brief calc_signal_b
//  * @param  raw(AD)
//  * @retval signal_b
//  */
//bool Demo::calc_signal_b(uint16_t raw){//raw < 4096
//    int _raw_calc_signal_b = ad_conv(raw);//YOKO STRETCH
//    return signal_pat_b[_raw_calc_signal_b];//raw2 < 1024
//}

/**
  * @brief デモ本体。メインループで毎回実行すること
  * @param  none
  * @retval none
  */
void Demo::loop(){
    static int cnt = 0;
    bool ret;
        
    switch(_step){
    case 0://通常デモ
        //設定データ更新されたら、通常デモを中断する。
        ret = ChangeCheck(false);
        if(ret){
            _tickCnt = 0;
            _step = 1;//設定データプロットへ
            break;
        }
    
        //1ms周期でモータ出力
        if(_tickCnt == 1){//1ms period
            _raw = _sensor->get_raw();//0-4095  150usec          
            if(_raw == 0xffff) {//i2c error
                _tickCnt = 0;
                break;//エラーの場合先に進まない
            }
            //モータ出力
            float pwm = ZeroTorqueCheck();
            _motor->pwmout(pwm);

//            //dummy encoder
//            *_signal_a = (calc_signal_a(_raw) > 0)? 1: 0;
//            
//            //正転
//            *_signal_b = (calc_signal_b(_raw) > 0)? 0: 1;

            //上記のものを下記で計算させる
            _wheel->calc(_raw2); //raw2は計算済み -> ZeroTorqueCheck

            //10ms周期でPWMプロット
            if(++cnt >= 10){//10ms period
                cnt = 0;
                
                _plot->plot(pwm);
            }
            
            if(_tickCnt >= 2) NOP();//2ms passed error
            
            _tickCnt = 0;
            
        }else if(_tickCnt >= 2){//2ms passed error
            NOP();
        }
        break;
        
    case 1://設定データのプロット期間
        ret = ChangeCheck(false);
        if(!ret){//設定データ変化なし
            //下記時間経過したら設定プロットをやめて、通常の処理へ戻る
            if(_tickCnt >= 20){//20msec passed  //ibuki 21.12.14 changed 3sec->20msec
                _tickCnt = 0;
                _step = 0;
            }
        } else {//設定データ更新された
            _tickCnt = 0;//タイマ計測リセット
        }
        break;
    default:
        break;
    }//end_switch
}

