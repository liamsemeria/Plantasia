# Plantasia
Plantasia is a basic computational language based on planting variables into the garden (a stack)

SYNTAX AND VOCABULARY
seed: (plant name) (plant int value)
    creates variable
    ex:
        seed rose 3
plant: (plant name)
    pushes plant onto garden
    ex:
        plant rose
propagate: (plant name)
    pushes top of garden + plant value
    if no plant it copies top
    ex:
        propagate
        or
        propagate rose

dig:
    pops plant from garden
    into (plant name) puts the popped value into the plant
    ex:
        dig into rose
        or
        dig

examine: (plant name)
    prints the plants value
    ex:
        examine rose

wither: (plant name)
    decrements plant until 0 and executes block untill end statement each iteration
    if the plant is 0 the block doesnt executes
    ex:
        wither rose
            examine rose
        end