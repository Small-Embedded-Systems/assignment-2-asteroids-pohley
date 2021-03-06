/**
*
* @section LICENSE
*
* Copyright (c) 2012 mbed
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* @section DESCRIPTION
*    A Driver set for the I2C half of the WM8731. Based on the WM8731
*    code http://mbed.org/users/p07gbar/code/WM8731/
*
*/ 

#include "WM8731.h"

#define WM8731_HP_VOL_DF_MASK 0x80


#define WM8731_DF_hp_vol_left     0.5
#define WM8731_DF_hp_vol_right    0.5
#define WM8731_DF_li_vol_left     0.5
#define WM8731_DF_li_vol_right    0.5
#define WM8731_DF_sdt_vol         0

const uint8_t base_address = 0x1A;


WM8731::WM8731(PinName i2c_sda, PinName i2c_scl): i2c(i2c_sda,i2c_scl)  {
    address = base_address;
    defaulter();
    form_cmd(all);
}

WM8731::WM8731(PinName i2c_sda, PinName i2c_scl, bool cs_level): i2c(i2c_sda,i2c_scl)  {
    address = base_address + (1*cs_level);
    defaulter();
    form_cmd(all);
}

void WM8731::power(bool on_off) {
    device_all_pwr = on_off;
    form_cmd(power_control);
}

void WM8731::input_select(int input) {

    switch(input)
    {
        case WM8731_NO_IN:
            device_adc_pwr = false;
            device_mic_pwr = false;
            device_lni_pwr = false;
            form_cmd(power_control);
            break;
        case WM8731_LINE:
            device_adc_pwr = true;
            device_lni_pwr = true;
            device_mic_pwr = false;
            ADC_source = WM8731_LINE;
            form_cmd(power_control);
            form_cmd(path_analog);
            break;
        case WM8731_MIC:
            device_adc_pwr = true;
            device_lni_pwr = true;
            device_mic_pwr = true;
            ADC_source = WM8731_MIC;
            form_cmd(power_control);
            form_cmd(path_analog);
            break;
        default:
            device_adc_pwr     = df_device_adc_pwr;
            device_mic_pwr     = df_device_mic_pwr;
            device_lni_pwr     = df_device_lni_pwr;
            ADC_source         = df_ADC_source;
            form_cmd(power_control);
            form_cmd(path_analog);
            break;
    }
    ADC_source_old = ADC_source;
}

void WM8731::headphone_volume(float h_volume) {
    hp_vol_left = h_volume;
    hp_vol_right = h_volume;
    form_cmd(headphone_vol_left);
    form_cmd(headphone_vol_right);
}

void WM8731::linein_volume(float li_volume) {
    li_vol_left = li_volume;
    li_vol_right = li_volume;
    form_cmd(line_in_vol_left);
    form_cmd(line_in_vol_right);
}

void WM8731::microphone_boost(bool mic_boost) {
    mic_boost_ = mic_boost;
}

void WM8731::input_mute(bool mute) {
    if(ADC_source == WM8731_MIC)
    {
        mic_mute = mute;
        form_cmd(path_analog);
    }
    else
    {
        li_mute_left = mute;
        li_mute_right = mute;
        form_cmd(line_in_vol_left);
        form_cmd(line_in_vol_right);
    }
}

void WM8731::output_mute(bool mute) {
    out_mute = mute;
    form_cmd(path_digital);
}

void WM8731::input_power(bool on_off) {
    
    device_adc_pwr = on_off;
    
    if(ADC_source == WM8731_MIC)
    {
        device_mic_pwr = on_off;
        device_lni_pwr = on_off;
    }
    else
    {
        device_mic_pwr = false;
        device_lni_pwr = on_off;
    }
    
    form_cmd(power_control);
}

void WM8731::output_power(bool on_off) {
    device_dac_pwr = on_off;
    device_out_pwr = on_off;
    
    form_cmd(power_control);
}

void WM8731::wordsize(int words) {
    device_bitlength = words;
    form_cmd(interface_format);
}

void WM8731::master(bool master) {
    device_master = master;
    form_cmd(interface_format);
}

void WM8731::frequency(int freq) {
    ADC_rate = freq;
    DAC_rate = freq;
    form_cmd(sample_rate);
}

void WM8731::input_highpass(bool enabled) {
    ADC_highpass_enable = enabled;
    form_cmd(path_digital);
}

void WM8731::output_softmute(bool enabled) {
    out_mute = enabled;
    form_cmd(path_digital);
}

void WM8731::interface_switch(bool on_off) {
    device_interface_active = on_off;
    form_cmd(interface_activation);
}

void WM8731::sidetone(float sidetone_vol) {
    sdt_vol = sidetone_vol;
    form_cmd(path_analog);
}

void WM8731::deemphasis(char code) {
    de_emph_code = code & 0x03;
    form_cmd(path_digital);
}

void WM8731::reset() {
    form_cmd(reset_reg);
}

void WM8731::start() {
    interface_switch(true);
}

void WM8731::bypass(bool enable) {
    bypass_ = enable;
    form_cmd(path_analog);
}

void WM8731::stop() {
    interface_switch(false);
}

void WM8731::command(reg_address add, uint16_t cmd) {
    char temp[2];
    temp[0] = (char(add)<<1) | ((cmd >> 6) & 0x01);
    temp[1] = (cmd & 0xFF);
    i2c.write((address<<1), temp, 2);
}

void WM8731::form_cmd(reg_address add) {
    uint16_t cmd = 0;
    int temp = 0;
    bool mute;
    switch(add)
    {
        case line_in_vol_left:
            temp = int(li_vol_left * 32) - 1;
            mute = li_mute_left;
            
            if(temp < 0) 
            {
                temp = 0; 
                mute = true;
            }
            cmd = temp & 0x1F;
            cmd |= mute << 7;
            break;
        case line_in_vol_right:
            temp = int(li_vol_right * 32) - 1;
            mute = li_mute_right;
            if(temp < 0) 
            {
                temp = 0; 
                mute = true;
            }
            cmd = temp & 0x1F;
            cmd |= mute << 7;
            break;
            
        case headphone_vol_left:
            temp = int(hp_vol_left * 80) + 47;
            cmd = WM8731_HP_VOL_DF_MASK;
            cmd |= temp & 0x7F;
            break;
        case headphone_vol_right:
            temp = int(hp_vol_right * 80) + 47;
            cmd = WM8731_HP_VOL_DF_MASK;
            cmd |= temp & 0x7F;
            break;
        
        case path_analog:
            temp = int(sdt_vol * 4);
            char vol_code = 0;
            switch(temp)
            {
                case 4:
                    vol_code = 0x01;
                    break;
                case 0:
                    vol_code = 0x00;
                    break;
                default:
                    vol_code = (((0x04 - temp)&0x03) << 1) | 0x01;
                    break;
            }
            cmd = vol_code << 5;
            cmd |= 1 << 4;
            cmd |= bypass_ << 3;
            cmd |= ADC_source << 2;
            cmd |= mic_mute << 1;
            cmd |= mic_boost_;
            break;
            
        case path_digital:
            cmd |= out_mute << 3;
            cmd |= ((de_emph_code & 0x3) << 1);
            cmd |= ADC_highpass_enable;
            break;
        
        case power_control:
            cmd |= !device_all_pwr << 7;
            cmd |= !device_clk_pwr << 6;
            cmd |= !device_osc_pwr << 5;
            cmd |= !device_out_pwr << 4;
            cmd |= !device_dac_pwr << 3;
            cmd |= !device_adc_pwr << 2;
            cmd |= !device_mic_pwr << 1;
            cmd |= !device_lni_pwr << 0;
            break;
            
        case interface_format:
            cmd |= device_master << 6;
            cmd |= device_lrswap << 5;
            cmd |= device_lrws     << 4;
            temp = 0;
            switch(device_bitlength)
            {
                case 16:
                    temp = 0;
                    break;
                case 20:
                    temp =  1;
                    break;
                case 24:
                    temp = 2;
                    break;
                case 32:
                    temp = 3;
                    break;
            }
            cmd |= (temp & 0x03) << 2;
            cmd |= (device_data_form & 0x03);
            break;
        
        case sample_rate:
            temp = gen_samplerate();
            cmd = device_usb_mode;
            cmd |= (temp & 0x1f) << 1;
            cmd |= device_clk_in_div << 6;
            cmd |= device_clk_out_div << 7;
            break;
            
        case interface_activation:
            cmd = device_interface_active;
            break;
            
        case reset_reg:
            cmd = 0;
            break;
        
        case all:
            for( int i = line_in_vol_left; i <= reset_reg; i++)
            {
                form_cmd((reg_address)i);
            }
            break;
    }
    if(add != all) command(add , cmd);
}

void WM8731::defaulter() {
    hp_vol_left = WM8731_DF_hp_vol_left;
    hp_vol_right = WM8731_DF_hp_vol_right;
    li_vol_left = WM8731_DF_li_vol_left;
    li_vol_right = WM8731_DF_li_vol_right;
    sdt_vol = WM8731_DF_sdt_vol;
    bypass_ = df_bypass_;
    
    ADC_source = df_ADC_source;
    ADC_source_old = df_ADC_source;
    
    mic_mute = df_mic_mute;
    li_mute_left = df_li_mute_left;
    li_mute_right = df_li_mute_right;
    
    
    mic_boost_ = df_mic_boost_;
    out_mute = df_out_mute;
    de_emph_code = df_de_emph_code;
    ADC_highpass_enable = df_ADC_highpass_enable;
    
    device_all_pwr = df_device_all_pwr;
    device_clk_pwr = df_device_clk_pwr;
    device_osc_pwr = df_device_osc_pwr;
    device_out_pwr = df_device_out_pwr;
    device_dac_pwr = df_device_dac_pwr;
    device_adc_pwr = df_device_dac_pwr;
    device_mic_pwr = df_device_mic_pwr;
    device_lni_pwr = df_device_lni_pwr;
    
    device_master       = df_device_master;
    device_lrswap       = df_device_lrswap;
    device_lrws         = df_device_lrws;
    device_bitlength    = df_device_bitlength;

    
    ADC_rate  = df_ADC_rate;
    DAC_rate  = df_DAC_rate;

    device_interface_active  = df_device_interface_active;
}

char WM8731::gen_samplerate() {
    char temp = 0;
    switch(ADC_rate)
    {
        case 96000:
            temp = 0x0E;
            break;
        case 48000:
            temp = 0x00;
            if(DAC_rate == 8000) temp = 0x02;
            break;
        case 32000:
            temp = 0x0C;
            break;
        case 8000:
            temp = 0x06;
            if(DAC_rate == 48000) temp = 0x04;
            break;
        default:
            temp = 0x00;
            break;
    }
    return temp;
}
