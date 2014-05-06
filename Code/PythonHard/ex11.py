# Note a virgula no final do print: isso evita uma nova-linha
print "How old are you?",
age = raw_input()

# Eh possivel usar o raw_input direto, mas ele nao coloca espaco depois
# da pergunta
height = raw_input("How tall are you?")
weight = raw_input("How much do you weight?")

enjoying = raw_input("Are you enjoying Python?")

print "So, you're %r old, %r tall and %r heavy." % (
	age, height, weight)

print "You said that 'You enjoy Python' is a %r statement." % enjoying

