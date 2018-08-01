----------------------------------------------------------------------------------
-- Engineer: Damy van Valenberg
-- 
-- Create Date:    14:54:56 1/8/2018
-- Design Name:    ALU
-- Module Name:    ALU - Behavioral 
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

entity ALU is
    Port ( 	x, y : in STD_LOGIC_VECTOR(15 downto 0);
			zx, nx, zy, ny, f, nq : in STD_LOGIC;
			q : out STD_LOGIC_VECTOR(15 downto 0);
			zr, ng : out STD_LOGIC
			 );
end ALU;

architecture Behavioral of ALU is
signal zx_i, nx_i, zy_i, ny_i, q_i, nq_i : STD_LOGIC_VECTOR(15 downto 0);
begin

-- Zero the inputs depending on the zero flags
zx_i <= x when (zx = '0') else x"0000";
zy_i <= y when (zy = '0') else x"0000";

-- Negate the inputs depending on the negate flags
nx_i <= zx_i when (nx = '0') else not zx_i;
ny_i <= zy_i when (ny = '0') else not zy_i;

-- Add or and the inputs depending on the function flag
q_i <= STD_LOGIC_VECTOR(unsigned(nx_i) + unsigned(ny_i)) when (f = '1') else nx_i and ny_i;

-- Negate the output depending on the negate flag
nq_i <= q_i when (nq = '0') else not q_i;

-- Set the outputs
q <= nq_i;
zr <= '1' when (nq_i = x"0000") else '0';
ng <= nq_i(15);

end Behavioral;

