#include <stdio.h>
#include <stdlib.h>
typedef struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
} retire_info;

void update_balance(int ageInMonths, double balance, retire_info info) {
  for (int i = 0; i < info.months; i++) {
    int ageYears = ageInMonths / 12;
    int ageMonths = ageInMonths % 12;

    printf("Age %3d month %2d you have $%.2f\n", ageYears, ageMonths, balance);
    balance += (balance * info.rate_of_return) + info.contribution;
    (ageInMonths)++;
  }
}
void retirement(int startAge,           //in months
                double initial,         //initial savings in dollars
                retire_info working,    //info about working
                retire_info retired) {  //info about being retired
  update_balance(startAge, initial, working);
  update_balance(startAge, initial, retired);
}

int main() {
  retire_info working = {489, 1000, 0.045 / 12};

  retire_info retired = {384, -4000, 0.01 / 12};
  int startAge = 327;
  double initialSavings = 21345;

  retirement(startAge, initialSavings, working, retired);

  return 0;
}
