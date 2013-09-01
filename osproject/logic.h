#define mainMAIN_LOGIC_CHECK_PERIOD		( ( portTickType ) 3000 / portTICK_RATE_MS  )

#define logicSTATE_IDLE			1
#define logicSTATE_HAS_CARD		2
#define logicSTATE_CHECKING		3
#define logicSTATE_BUYING		4

void vMainLogic(void *pvParameters );
