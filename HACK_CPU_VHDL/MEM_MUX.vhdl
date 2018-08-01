----------------------------------------------------------------------------------
-- Engineer: Damy van Valenberg
-- 
-- Create Date:    14:54:56 1/8/2018
-- Design Name:    MEM_MUX
-- Module Name:    MEM_MUX - Behavioral 
-- Project Name:   HACK CPU
-- Target Devices: LCMXO2
-- Tool versions:  
-- Description:    Memory port multiplexer. Creates a dual port ram interface from single port RAM.
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

entity MEM_MUX is
    Port ( 	clk : in STD_LOGIC;
			-- Interface
			data_a, data_b : in STD_LOGIC_VECTOR(15 downto 0);
			addr_a, addr_b : in STD_LOGIC_VECTOR(14 downto 0);
			we_a, we_b : in STD_LOGIC;
			q_a, q_b : out STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
			
			-- Physical RAM chip controls
			EXT_RAM_Q : in STD_LOGIC_VECTOR(15 downto 0);
			EXT_RAM_D : out STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
			EXT_RAM_A : out STD_LOGIC_VECTOR(14 downto 0) := (others => '0');
			EXT_RAM_W : out STD_LOGIC := '0'
			 );
end MEM_MUX;


architecture Behavioral of MEM_MUX is
signal switch : STD_LOGIC := '0'; -- Switch = 0 -> A, switch = 1 -> B
signal write_a_cache, write_b_cache, read_a_cache, read_b_cache : STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
signal addr_a_cache, addr_b_cache : STD_LOGIC_VECTOR(14 downto 0) := (others => '0');
signal write_a_f, write_b_f : STD_LOGIC := '0';
begin

q_a <= read_a_cache;
q_b <= read_b_cache;

process(switch)
begin
if (switch = '0') then
	if (write_a_f = '0') then
		EXT_RAM_A <= addr_a;
	else
		EXT_RAM_A <= addr_a_cache;
	end if;
else
	if (write_b_f = '0') then
		EXT_RAM_A <= addr_b;
	else
		EXT_RAM_A <= addr_b_cache;
	end if;
end if;

end process;

process(clk)
begin
if (clk'event and clk = '0') then
	-- Switch port select
	switch <= not switch;
	
	-- Control the write and data lines accordingly. Read when switching. Reset the write flags
	if (switch = '1') then
		read_b_cache <= EXT_RAM_Q;
		EXT_RAM_D <= write_a_cache;
		EXT_RAM_W <= write_a_f;
		write_b_f <= '0';
	else
		read_a_cache <= EXT_RAM_Q;
		EXT_RAM_D <= write_b_cache;
		EXT_RAM_W <= write_b_f;
		write_a_f <= '0';
	end if;
	
	-- Cache write operations
	if (we_a = '1') then
		addr_a_cache <= addr_a;
		write_a_cache <= data_a;
		write_a_f <= '1';
	end if;
	if (we_b = '1') then
		addr_b_cache <= addr_b;
		write_b_cache <= data_b;
		write_b_f <= '1';
	end if;
end if;
end process;

end Behavioral;

