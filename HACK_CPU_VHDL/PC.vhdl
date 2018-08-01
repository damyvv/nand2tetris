----------------------------------------------------------------------------------
-- Engineer: Damy van Valenberg
-- 
-- Create Date:    14:54:56 1/8/2018
-- Design Name:    PC
-- Module Name:    PC - Behavioral 
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

entity PC is
    Port ( 	addr : in STD_LOGIC_VECTOR(15 downto 0);
			clk, inc, load, reset : in STD_LOGIC;
			q : out STD_LOGIC_VECTOR(15 downto 0)
			 );
end PC;

architecture Behavioral of PC is
signal count : unsigned (15 downto 0) := (others => '0');
begin

q <= STD_LOGIC_VECTOR(count);

process(clk)
begin
if (clk'event and clk = '0') then

	if (reset = '1') then
		count <= (others => '0');
	elsif (load = '1') then
		count <= unsigned(addr);
	elsif (inc = '1') then
		count <= count + 1;
	end if;

end if;
end process;

end Behavioral;

