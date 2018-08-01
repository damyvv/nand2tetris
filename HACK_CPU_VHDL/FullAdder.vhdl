----------------------------------------------------------------------------------
-- Engineer: Damy van Valenberg
-- 
-- Create Date:    13:55:45 1/8/2018
-- Design Name:    FullAdder
-- Module Name:    FullAdder - Behavioral 
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

entity FullAdder is
    Port ( a,b,c : in STD_LOGIC;
			sum, carry : out STD_LOGIC
			 );
end FullAdder;

architecture Behavioral of FullAdder is
signal t_sum : unsigned(1 downto 0); -- Create a temporary unsigned signal
begin
	
t_sum <= unsigned'("0"&a) + unsigned'("0"&b) + unsigned'("0"&c);

sum <= t_sum(0);
carry <= t_sum(1);

end Behavioral;

