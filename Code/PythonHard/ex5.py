Name = "Zed A. Shaw"
Age = 35 # not a lie
Height = 74 # inches
Weight = 180 # lbs
Eyes = 'Blue'
Teeth = 'White'
Hair = 'Brown'
KgFactor = 0.45359237
CmFactor = 2.54
Weight2 = Weight * KgFactor
Height2 = Height * CmFactor

print "Let's talk about %s." % Name
print "He's %d inches tall (or %d cm)." % ( Height , Height2 )
print "He's %d pounds heavy (or %d kg)." % ( Weight , Weight2 )
print "Actually, that's not too heavy."
print "He's got %s eyes and %s hair." % ( Eyes, Hair )
print "His teeth are usually %s depending on the coffee." % Teeth

# This line is tricky, try to get it exactly right
print "If I add %d, %d and %d I get %d." % (
	Age, Height, Weight, Age + Height + Weight )

print "Print %r, no matter what." % Name
