#include <stdio.h>
#include <stdlib.h>
typedef struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
} retire_info;

void update_balance(int * ageInMonths, double * balance, retire_info info) {
  for (int i = 0; i < info.months; i++) {
    int ageYears = *ageInMonths / 12;
    int ageMonths = *ageInMonths % 12;

    printf("Age %3d month %2d you have $%.2f\n", ageYears, ageMonths, *balance);
    *balance += (*balance * info.rate_of_return) + info.contribution;
    (*ageInMonths)++;
  }
}
void retirement(int startAge,           //in months
                double initial,         //initial savings in dollars
                retire_info working,    //info about working
                retire_info retired) {  //info about being retired
  update_balance(&startAge, &initial, working);
  update_balance(&startAge, &initial, retired);
}

int main() {
  retire_info working = {489, 1000, 0.045 / 12};
  // 40 years working, contributing $1000 per month, 4.5% annual return

  // Retirement phase information
  retire_info retired = {384, -4000, 0.01 / 12};
  // 32 years retired, withdrawing $4000 per month, 1% annual return

  // Starting conditions
  int startAge = 327;
  // Start at age 27 years and 3 months (327 months)
  double initialSavings = 21345;
  // Initial savings of $21,345

  // Calculate retirement plan
  retirement(startAge, initialSavings, working, retired);

  return 0;
}
