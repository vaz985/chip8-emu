#include "c8.h"

void chip8::insert_rom() {
	std::string rom_dir;
	std::ifstream rom("pong2.c8", std::ifstream::binary);
	//std::cout << "Input rom dir: " << std::endl;
	//while(rom.good()) {
	//	std::cout << "try" << std::endl;
	//	std::cin >> rom_dir;
	//	rom.open(rom_dir, std::ifstream::binary);	
	//}
	rom.seekg(0, std::ios_base::end);
	int lenght = rom.tellg();
	rom.seekg(0, std::ios_base::beg);

	char * buffer = new char [lenght];
	rom.read(buffer, lenght);
	for(int i=0; i<80; i++) {
		this->memory[512 + i] = buffer[i];
	}
}
void chip8::initialize() {
	insert_rom();

	// Init RNG
	this->rng.seed(std::random_device()());

	// Clean
	this->pc		 = 0x200; // 
	this->opcode = 0;			//
	this->I			 = 0;			//
	this->sp		 = 0;			//
	
	for(int i=0; i<80; i++) {
		this->memory[i] = chip8_fontset[i];
	}
	// reset timers
}


void chip8::emulateCycle() {
	// Fetch
	this->opcode = this->memory[pc] << 8 | this->memory[pc+1];
	// Decode & Execute
	switch(this->opcode & 0xF000) {

		case 0x0000:
			switch(opcode & 0x00FF) {
				// 0x00E0: Clears the screen.
				case 0x00E0:
					//TODO
					std::cout << "Clear screen" << std::endl;
				break;
				// 0x00EE: Returns from a subroutine.
				case 0x00EE:
					this->pc = this->stack[this->sp--];
				break;
			
			}
		break;

		// 1NNN: Jumps to adress NNN.
		case 0x1000:
			this->pc = opcode & 0x0FFF;
		break;

		// 2NNN: Call subroutine at NNN.
		case 0x2000:
			this-stack[++this->sp] = this->pc;
		break;

		// 3XKK: Skip if Vx == KK
		case 0x3000:
			if(this->V[this->opcode & 0x0F00] == 0x00FF)
				this->pc += 2;
		break;

		// 4XKK: Skip if Vx != KK
		case 0x4000:
			if(this->V[this->opcode & 0x0F00] != 0x00FF)
				this->pc += 2;
		break;

		// 5XY0: Skip if Vx == Vy
		case 0x5000:
			if(this->V[this->opcode & 0x0F00] == this->V[this->opcode & 0x00F0])
				this->pc += 2;
		break;

		// 6XKK: Set Vx = KK
		case 0x6000:
			this->V[this->opcode & 0x0F00] += this->opcode & 0x00FF;
		break;

		// 7XKK: ADD Vx, byte
		case 0x7000:
			this->V[this->opcode & 0x0F00] = this->opcode & 0x00FF;
		break;

		case 0x8000:
			switch(opcode & 0x000F) {
				// 8XY0: LD Vx, Vy
				case 0x0000:
			}
		break;

		// ANNN: Sets I to the address NNN
		case 0xA000: 			
			this->I = this->opcode & 0x0FFF;
			this->pc += 2;
		break;

		// BNNN: Jumps to the address NNN plus V0.
		case 0xB000: 
			this->pc = this->V[0] + (this->opcode & 0x0FFF);
		break;

		// CNNN: Sets VX to the result of a bitwise and operation on a random 
		// number (Typically: 0 to 255) and NN. 
		case 0xC000:
			std::uniform_int_distribution<std::mt19937::result_type> dist(0,15);
			unsigned char X = this->opcode & 0x0F00;
			unsigned char rand = dist(this->rng);
		break;

	}

	// Update timers
}
