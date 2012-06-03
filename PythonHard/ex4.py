# O numero de carros
cars = 100
# O espaco dentro de apenas um carro
space_in_a_car = 4.0
# O numero de motoristas
drivers = 30
# O numero de passageiros
passengers = 90
# O numero de carros que nao podem ser digiridos (pq faltam motoristas)
cars_not_driven = cars - drivers
# Numero de carros dirigidos eh igual ao numero de motoristas
cars_driven = drivers
# O espaco total de todos os carros dirigidos
carpool_capacity = cars_driven * space_in_a_car
# O numero de passageiros que devem ser espalhados nos carros
average_passengers_per_car = passengers / cars_driven

print "There are", cars, "cars available."
print "There are only", drivers, "drivers available."
print "There will be", cars_not_driven, "empty cars today."
print "We can transport", carpool_capacity, "people today."
print "We have", passengers, "to carpool today."
print "We need to put about", average_passengers_per_car, "in each car."

