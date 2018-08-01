----------------------------------------------------------------------------------
-- Engineer: Damy van Valenberg
-- 
-- Create Date:    14:54:56 1/8/2018
-- Design Name:    CPU
-- Module Name:    CPU - Behavioral 
-- Project Name:   HACK CPU
-- Target Devices: LCMXO2
-- Tool versions:  
-- Description:    
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity CPU is
    Port ( 	RAM_D, ROM_D : in STD_LOGIC_VECTOR(15 downto 0);
			reset, clk : in STD_LOGIC;
			RAM_Q : out STD_LOGIC_VECTOR(15 downto 0);
			RAM_W : out STD_LOGIC;
			RAM_A, ROM_A : out STD_LOGIC_VECTOR(14 downto 0)
			 );
end CPU;


architecture Behavioral of CPU is
signal unused : STD_LOGIC;

---- Aliases for the instructions ----
alias instruction : STD_LOGIC_VECTOR(15 downto 0) is ROM_D;
-- A/C instruction flag
alias C_inst : STD_LOGIC is instruction(15);
-- A/M register select flag
alias AM_SEL : STD_LOGIC is instruction(12);

-- Computate flags for ALU
alias zx : STD_LOGIC is instruction(11);
alias nx : STD_LOGIC is instruction(10);
alias zy : STD_LOGIC is instruction(9);
alias ny : STD_LOGIC is instruction(8);
alias f : STD_LOGIC is instruction(7);
alias nq : STD_LOGIC is instruction(6);

-- ALU output destination flags
alias dest_A : STD_LOGIC is instruction(5);
alias dest_D : STD_LOGIC is instruction(4);
alias dest_M : STD_LOGIC is instruction(3);

-- ALU jump condition flags
alias jmp_n : STD_LOGIC is instruction(2);
alias jmp_z : STD_LOGIC is instruction(1);
alias jmp_g : STD_LOGIC is instruction(0);
---- End of aliases ----

signal D_REG, A_REG : STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
signal AM_MUX : STD_LOGIC_VECTOR(15 downto 0);
signal ALU_q : STD_LOGIC_VECTOR(15 downto 0);
signal ALU_zr, ALU_ng : STD_LOGIC;
signal jmp : STD_LOGIC;
begin

-- Select A or M register depending on the instruction
AM_MUX <= RAM_D when (AM_SEL = '0') else A_REG;

-- Process jump condition flags
jmp <= (jmp_n and ALU_ng) or (jmp_z and ALU_zr) or (jmp_g and not (ALU_zr and ALU_ng));

-- Write RAM flag
RAM_W <= dest_M and C_inst;

registers_set : process(clk)
begin
if (clk'event and clk = '0') then
	if (C_inst = '1') then -- C instruction
		if (dest_D = '1') then
			D_REG <= ALU_q; -- Load the ALU output in the D register
		end if;
		if (dest_A = '1') then
			A_REG <= ALU_q; -- Load the ALU output in the A register
		end if;
	else -- A instruction
		A_REG <= ROM_D; -- Load the ROM output in the A register
	end if;
end if;
end process;

ALU : entity work.ALU port map (
		x  => D_REG,
		y  => AM_MUX,
		zx => zx,
		nx => nx,
		zy => zy,
		ny => ny,
		f  => f,
		nq => nq,
		q  => ALU_q,
		zr => ALU_zr,
		ng => ALU_ng
	);
	
PC : entity work.PC port map (
		addr  => A_REG,
		clk   => clk,
		inc   => '1',
		load  => jmp,
		reset => reset,
		q(15) => unused,
		q(14 downto 0) => ROM_A
	);
	

-- Connect the ALU output to the RAM data bus
RAM_Q <= ALU_q;

-- Connect the A register to the RAM address bus
RAM_A <= A_REG(14 downto 0);

end Behavioral;

