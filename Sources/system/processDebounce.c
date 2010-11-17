#include "processDebounce.h"

void ProcessDebounce()
{
	static u08 state;
	if (GetMessage(MSG_DEBOUNCE))
	{

		SendMessage(MSG_B_KEY_PRESSED);
	}
}

/*







 else
 {
 switch (state)
 {
 case 0:
 if (keyCode > '1')
 {
 state = 0;
 SendMessage(MSG_B_KEY_PRESSED);
 StartTimer(TIMER_4);
 }
 break;
 case 1:
 if (keyCode <= '1')
 {
 state = 2;
 }
 else if (GetTimer(TIMER_4) >= 0.5 * sec)
 {
 SendMessage(MSG_B_KEY_PRESSED);
 StartTimer(TIMER_4);
 }
 break;
 case 2:
 if (keyCode == '0')
 {
 state = 0;
 }
 break;
 case 3:
 break;
 default:
 break;
 }
 }
 }
 }

 */
