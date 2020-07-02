int main(){

    int message[] = {83, 97, 105, 115, 105, 114, 32, 53, 32, 108, 101, 116, 116, 114, 101, 115, 32, 97, 32, 116, 114, 105, 101, 114, 10};
    int i = 0;
    for(i = 0; i < 25; i++){
        putchar(message[i]);
    }

    int t[5];

    for(i = 0; i < 5; i++){
        t[i] = getchar();
    }

    int en_desordre = 1;
    int j;
    int tmp;
	while (en_desordre)
	{
		en_desordre = 0;
		for (j = 0; j < 4; j++)
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

    putchar(10);

    for(i = 0; i < 5; i++){
        putchar(t[i]);
        putchar(10);
    }

    return 0;
}