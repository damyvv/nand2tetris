library ieee;
use ieee.NUMERIC_STD.all;
use ieee.std_logic_1164.all;

	-- Add your library and packages declaration here ...

entity FullAdder_TB is
end FullAdder_TB;

architecture TB_ARCHITECTURE of FullAdder_TB is
	-- Component declaration of the tested unit
	component fulladder
	port(
		a : in STD_LOGIC;
		b : in STD_LOGIC;
		c : in STD_LOGIC;
		sum : out STD_LOGIC;
		carry : out STD_LOGIC );
	end component;

	-- Stimulus signals - signals mapped to the input and inout ports of tested entity
	signal VECTOR : STD_LOGIC_VECTOR(2 downto 0);
	-- Observed signals - signals mapped to the output ports of tested entity
	signal sum : STD_LOGIC;
	signal carry : STD_LOGIC;
												 
begin

	-- Unit Under Test port map
	UUT : fulladder
		port map (
			a => VECTOR(0),
			b => VECTOR(1),
			c => VECTOR(2),
			sum => sum,
			carry => carry
		);
		
	process
	begin
		VECTOR <= "000";
		wait for 10 ns;
		
		for i in 0 to 7 loop
			VECTOR <= STD_LOGIC_VECTOR(unsigned(VECTOR) + 1);
			wait for 10 ns;
		end loop;
		
		wait;
	end process;

end TB_ARCHITECTURE;

