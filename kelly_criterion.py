balance = 100
desired_profit = float(input('desired profit : '))/100
desired_cutloss = float(input('desired cutloss : -'))/100
probability_profit = 0.5
probability_loss = 0.5
repetition = 4

# ? Kelly Criterion Equation
# * note return how many percentage balance is supposed to be tradable
percentage = probability_profit/desired_cutloss - probability_loss/desired_profit

# ? Median Return
all_in = balance
criterion = 0
for i in range(repetition//2):
    all_in = all_in * (1 + desired_profit)
    all_in = all_in * (1 - desired_cutloss)

    balance = balance + criterion
    criterion = balance * percentage * (1 + desired_profit)
    balance = balance * (1 - percentage)

    balance = balance + criterion
    criterion = balance * percentage * (1 - desired_cutloss)
    balance = balance * (1 - percentage)

print(f"all in median : {all_in}")
print(f"criterion median : {balance + criterion}")
