/**
  ******************************************************************************
  * @file           : Terminal.cpp
  * @brief          : Terminal (PC TeraTerm)
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "Terminal.h"
#include "common.h"

/**
  * @brief rx interrupt
  * @param  none
  * @retval none
  */
void Terminal::rx_irq(){
    char c = _pc->getc();
    _que->Put(&c);
}
/**
  * @brief constructor
  * @param  pc:serial port
  * @retval none
  */
Terminal::Terminal(Serial *pc){
    _pc = pc;
    _pc->attach(this, &Terminal::rx_irq, Serial::RxIrq);
    
    _step = 0;
    _que = new Queue(sizeof(char), 256);
    
}
/**
  * @brief idle state?
  * @param  none
  * @retval true is idle
  */
bool Terminal::IsIdle(){
    return _step == 0;
}
/**
  * @brief timer interrupt
  * @param  none
  * @retval none
  */
void Terminal::t_interrupt(){
    _timeout = true;
}
/**
  * @brief loop
  * @param  none
  * @retval none
  */
void Terminal::loop(){
    int n = _que->GetNumberOfItems();
    
    if(n == 0 && _timeout){
        _timeout = false;
        error();
    }
    
    while(n > 0){
        char c;
        _que->Get(&c);
//pc.putc(c);n--;continue;
        
        //STATE MACHINE CSV
        //N VAL VAL VAL VAL
        //0,127,127,127,127[CR][LF]   --> begin_write(), write()
        //1,127,127,127,127[CR][LF]   --> write()
        //10,127,127,127,127[CR][LF]
        //1023,127,127,127,127[CR][LF] --> write(), end_write()
        switch(_step){
/* RX START ------------------------------------------------------------------*/
        case 0://IDLE
            if(c == '0'){//N start
                _table.begin_write();
                _row = 0;
                _col = 0;
                _p = _csv;
                *_p++ = c;//buf update
                _timeout = false;
                _t.attach(this, &Terminal::t_interrupt, 0.1);
                _step++;
            }
            break;
        case 1://N
            
            _t.detach();
            if(c == ','){//N comma
                _col++;
                *_p++ = NULL;
                int row = atoi((char*)_csv);
                if(row == _row){//row match
                    _p = _csv;
                    _step++;//goto VAL
                } else {//row unmatch
                    _step = 9;
                }
            } else if(0x30 <= c && c <= 0x39){
                *_p++ = c;//buf update
            } else {
                _step = 9;
            }
            break;
        case 2://VAL
            if(c == CR){               
                _step++;//goto LF
                
            } else if(c == ','){//VAL comma
                _col++;
                *_p++ = NULL;
                int val = atoi((char*)_csv);
                getTwosComplement(&val, 8);
                int8_t x = val;
                _table.write((uint8_t)x);
                _p = _csv;
            } else if((0x30 <= c && c <= 0x39) || c == '-'){
                *_p++ = c;//buf update
            } else {
                _step = 9;
            }
            break;
        case 3://CR
            if(c == LF){
                if(++_col != COL_MAX) {
                    _step = 9;                    
                } else {
                    *_p++ = NULL;
                    int val = atoi((char*)_csv);
                    getTwosComplement(&val, 8);
                    int8_t x = val;
                    _table.write((uint8_t)x);
                    
                    if(++_row >= ROW_MAX){//EOF
                        dummy_read();
                        /*--- with rx, can't work ----------------------------*/
                        _table.end_write();//flash erase & write
                        /*----------------------------------------------------*/
                        _step = 0;
                        
                        _pc->printf("OK\n");
                        
                    } else {
                        //goto next line
                        _col = 0;
                        _p = _csv;
                        _timeout = false;
                        _t.attach(this, &Terminal::t_interrupt, 0.1);
                        _step = 1;
                    }
                }
            }else{//not CR
                _step = 9;
            }
            break;
            
        case 9://error
            error();            
            break;
/* RX END --------------------------------------------------------------------*/

        default:
            break;
        }//end_switch
        n--;
    }//end_getc
}
/**
  * @brief error state
  * @param  none
  * @retval none
  */
void Terminal::error(){
    dummy_read();
    _pc->printf("FORMAT NG\n");
    _step = 0;
}
/**
  * @brief dummy read
  * @param  none
  * @retval none
  */
void Terminal::dummy_read(){
    do {
        wait(0.1);
        int n = _que->GetNumberOfItems();
        if(n == 0) return;
        
        while(n > 0){
            char c;
            _que->Get(&c);//dummy read
            n--;
         }
    } while(1);
}

