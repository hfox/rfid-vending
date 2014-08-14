#define mainMAIN_LOGIC_CHECK_PERIOD		( ( portTickType ) 100 / portTICK_RATE_MS  )

#define logicSTATE_IDLE			1
#define logicSTATE_HAS_CARD		2
#define logicSTATE_CHECKING		3
#define logicSTATE_BUYING		4

void logic_run(void *pvParameters );
void logic_step(void);
void logic_init(void);

char logic_running(void);
