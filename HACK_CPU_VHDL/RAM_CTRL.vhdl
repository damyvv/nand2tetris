----------------------------------------------------------------------------------
-- Engineer: Damy van Valenberg
-- 
-- Create Date:    14:54:56 1/8/2018
-- Design Name:    RAM_CTRL
-- Module Name:    RAM_CTRL - Behavioral 
-- Project Name:   HACK CPU
-- Target Devices: LCMXO2
-- Tool versions:  
-- Description:    External RAM controller
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

entity RAM_CTRL is
    Port ( 	clk, write_clk : in STD_LOGIC;
			RAM_A : in STD_LOGIC_VECTOR(14 downto 0);
			RAM_W : in STD_LOGIC;
			RAM_D : in STD_LOGIC_VECTOR(15 downto 0);
			RAM_Q : out STD_LOGIC_VECTOR(15 downto 0);
			
			-- External RAM connections
			RAM_EXT_D : inout STD_LOGIC_VECTOR(15 downto 0);
			RAM_EXT_A : out STD_LOGIC_VECTOR(14 downto 0);
			RAM_EXT_W : out STD_LOGIC := '0';
			RAM_EXT_NOE : out STD_LOGIC := '1'
			 );
end RAM_CTRL;


architecture Behavioral of RAM_CTRL is
signal action : unsigned(1 downto 0) := (others => '0');
signal write_cache, read_cache : STD_LOGIC_VECTOR(15 downto 0) := (others => 'Z');
signal write_f : STD_LOGIC;
begin

RAM_EXT_A <= RAM_A;
RAM_Q <= read_cache;

process(write_clk)
begin
if (write_clk'event and write_clk = '0') then
	if (RAM_W = '1') then
		write_cache <= RAM_D;
		write_f <= '1';
	else
		write_f <= '0';
	end if;
end if;
end process;

process(clk)
begin
if (clk'event and clk = '0') then
	action <= action + 1;
	if (action = 0) then -- Input state
		RAM_EXT_NOE <= '0';
	elsif (action = 1) then -- Save input
		read_cache <= RAM_EXT_D;
		RAM_EXT_NOE <= '1';
	elsif (action = 2) then -- Output state
		RAM_EXT_D <= write_cache;
		RAM_EXT_W <= write_f;
	else
		RAM_EXT_W <= '0';
		RAM_EXT_D <= (others => 'Z');
	end if;
end if;
end process;

end Behavioral;

