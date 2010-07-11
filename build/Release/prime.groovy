def x = 50000000
def i = 2
def factors = 0
while(i < x) {
	if(x % i == 0) {
		factors = factors + 1
	}
	i = i + 1
}

println "Factors = " + factors
