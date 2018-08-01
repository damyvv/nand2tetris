----------------------------------------------------------------------------------
-- Engineer: Damy van Valenberg
-- 
-- Create Date:    14:54:56 1/8/2018
-- Design Name:    TOP
-- Module Name:    TOP - Behavioral 
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

entity TOP is
    Port ( 	-- ROM
			ROM_D : in STD_LOGIC_VECTOR(15 downto 0);
			ROM_A : out STD_LOGIC_VECTOR(14 downto 0);
	
			-- RAM
			RAM_EXT_D : inout STD_LOGIC_VECTOR(15 downto 0);
			RAM_EXT_A : out STD_LOGIC_VECTOR(14 downto 0);
			RAM_EXT_W : out STD_LOGIC;
			RAM_EXT_NOE : out STD_LOGIC;
			
			reset, clk : in STD_LOGIC
			 );
end TOP;


architecture Behavioral of TOP is
signal CPU_RAM_A, RAM_CTRL_A : STD_LOGIC_VECTOR(14 downto 0);
signal CPU_RAM_W, RAM_CTRL_W : STD_LOGIC;
signal CPU_RAM_Q, CPU_RAM_D, RAM_CTRL_Q, RAM_CTRL_D : STD_LOGIC_VECTOR(15 downto 0);
signal clk_div : unsigned(2 downto 0) := (others => '0');
begin

CPU : entity work.CPU port map (
		RAM_D => CPU_RAM_Q,
		ROM_D => ROM_D,
		reset => reset,
		clk   => clk_div(2),
		RAM_Q => CPU_RAM_D,
		RAM_W => CPU_RAM_W,
		RAM_A => CPU_RAM_A,
		ROM_A => ROM_A
	);
	
RAM_CTRL : entity work.RAM_CTRL port map (
		clk         => clk,
		write_clk   => clk_div(0),
		RAM_A       => RAM_CTRL_A,
		RAM_W       => RAM_CTRL_W,
		RAM_D       => RAM_CTRL_D,
		RAM_Q       => RAM_CTRL_Q,
		RAM_EXT_D   => RAM_EXT_D,
		RAM_EXT_A   => RAM_EXT_A,
		RAM_EXT_W   => RAM_EXT_W,
		RAM_EXT_NOE => RAM_EXT_NOE
	);
	
MEM_MUX : entity work.MEM_MUX port map (
		clk       => clk_div(0),
		data_a    => CPU_RAM_D,
		data_b    => (others => '0'),
		addr_a    => CPU_RAM_A,
		addr_b    => (others => '0'),
		we_a      => CPU_RAM_W,
		we_b      => '0',
		q_a       => CPU_RAM_Q,
--		q_b       => 
		EXT_RAM_Q => RAM_CTRL_Q,
		EXT_RAM_D => RAM_CTRL_D,
		EXT_RAM_A => RAM_CTRL_A,
		EXT_RAM_W => RAM_CTRL_W
	);
	
clk_divider : process(clk)
begin
if (clk'event and clk = '0') then
	clk_div <= clk_div + 1;
end if;
end process;

end Behavioral;

