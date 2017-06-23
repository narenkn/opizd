#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "gpio_lib.c"

#define  PORT SUNXI_GPA(12)
int main(){

  int ret;

  ret = sunxi_gpio_init();
  ret = sunxi_gpio_set_cfgpin(PORT, SUNXI_GPIO_OUTPUT);

  for (uint32_t ui1=10; ui1; ui1--) {
    sunxi_gpio_output(PORT, 1);
    sleep(2);
    sunxi_gpio_output(PORT, 0);
    sleep(2);
  }

  ret = sunxi_gpio_set_cfgpin(PORT, SUNXI_GPIO_INPUT);
}
