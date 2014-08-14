#define mainTEST_CHECK_PERIOD		( ( portTickType ) 100 / portTICK_RATE_MS  )

void test_run(void *pvParameters );
void test_step(void);
void test_init(void);

char test_running(void);

