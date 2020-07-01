int main(){
getchar();
    int message[] = {83, 97, 105, 115, 105, 114, 32, 53, 32, 108, 101, 116, 116, 114, 101, 115, 32, 97, 32, 116, 114, 105, 101, 114, 13};
    int i;
    for(i = 0; i < 25; i++){
        putchar(message[i]);
    }
getchar();
getchar();
    int t[5];

    for(i = 0; i < 5; i++)
        t[i] = getchar();

    int j;
    int tmp;
    int en_desordre = 1; 
    
	while (en_desordre)
	{
		en_desordre = 0;
		for (int j = 0; j < 4; j++)
		{
			if(t[j] > t[j+1])
			{
 				tmp = t[j+1];
 				t[j+1] = t[j];
 				t[j] = tmp;

				en_desordre = 1;
 			}
		}
	}

    for(i = 0; i<5;i++){
        putchar(t[0]);
        putchar(10);
    }

    return 0;
}