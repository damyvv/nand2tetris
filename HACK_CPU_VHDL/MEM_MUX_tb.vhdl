
-- VHDL Test Bench Created from source file MEM_MUX.vhd -- Wed Aug 01 21:15:20 2018

--
-- Notes: 
-- 1) This testbench template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the unit under test.
-- Lattice recommends that these types always be used for the top-level
-- I/O of a design in order to guarantee that the testbench will bind
-- correctly to the timing (post-route) simulation model.
-- 2) To use this template as your testbench, change the filename to any
-- name of your choice with the extension .vhd, and use the "source->import"
-- menu in the ispLEVER Project Navigator to import the testbench.
-- Then edit the user defined section below, adding code to generate the 
-- stimulus for your design.
-- 3) VHDL simulations will produce errors if there are Lattice FPGA library 
-- elements in your design that require the instantiation of GSR, PUR, and
-- TSALL and they are not present in the testbench. For more information see
-- the How To section of online help.  
--
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY MEM_MUX_TB IS
END MEM_MUX_TB;

ARCHITECTURE behavior OF MEM_MUX_TB IS 

	COMPONENT MEM_MUX
	PORT(
		clk : IN std_logic;
		data_a : IN std_logic_vector(15 downto 0);
		data_b : IN std_logic_vector(15 downto 0);
		addr_a : IN std_logic_vector(14 downto 0);
		addr_b : IN std_logic_vector(14 downto 0);
		we_a : IN std_logic;
		we_b : IN std_logic;
		EXT_RAM_Q : IN std_logic_vector(15 downto 0);          
		q_a : OUT std_logic_vector(15 downto 0);
		q_b : OUT std_logic_vector(15 downto 0);
		EXT_RAM_D : OUT std_logic_vector(15 downto 0);
		EXT_RAM_A : OUT std_logic_vector(14 downto 0);
		EXT_RAM_W : OUT std_logic
		);
	END COMPONENT;

	SIGNAL clk :  std_logic := '0';
	SIGNAL data_a :  std_logic_vector(15 downto 0) := (others => '0');
	SIGNAL data_b :  std_logic_vector(15 downto 0) := (others => '0');
	SIGNAL addr_a :  std_logic_vector(14 downto 0) := "000" & x"005";
	SIGNAL addr_b :  std_logic_vector(14 downto 0) := (others => '0');
	SIGNAL we_a :  std_logic := '0';
	SIGNAL we_b :  std_logic := '0';
	SIGNAL q_a :  std_logic_vector(15 downto 0);
	SIGNAL q_b :  std_logic_vector(15 downto 0);
	SIGNAL EXT_RAM_Q :  std_logic_vector(15 downto 0);
	SIGNAL EXT_RAM_D :  std_logic_vector(15 downto 0) := (others => '0');
	SIGNAL EXT_RAM_A :  std_logic_vector(14 downto 0) := (others => '0');
	SIGNAL EXT_RAM_W :  std_logic := '0';

BEGIN

-- Please check and add your generic clause manually
	uut: MEM_MUX PORT MAP(
		clk => clk,
		data_a => data_a,
		data_b => data_b,
		addr_a => addr_a,
		addr_b => addr_b,
		we_a => we_a,
		we_b => we_b,
		q_a => q_a,
		q_b => q_b,
		EXT_RAM_Q => EXT_RAM_Q,
		EXT_RAM_D => EXT_RAM_D,
		EXT_RAM_A => EXT_RAM_A,
		EXT_RAM_W => EXT_RAM_W
	);

	clock : process
	begin				 
		wait for 2 ns;
		clk <= not clk;	  
	end process;


-- *** Test Bench - User Defined Section ***
	external_memory : PROCESS (EXT_RAM_A)
	BEGIN
	   if (EXT_RAM_A = "000" & x"001") then
		   EXT_RAM_Q <= x"0005";
	   elsif ( EXT_RAM_A = "000" & x"002" ) then
		   EXT_RAM_Q <= x"0006";
	   elsif ( EXT_RAM_A = "000" & x"003" ) then
		   EXT_RAM_Q <= x"0007";
	   elsif ( EXT_RAM_A = "000" & x"004" ) then
		   EXT_RAM_Q <= x"0008";
	   elsif ( EXT_RAM_A = "000" & x"005" ) then
		   EXT_RAM_Q <= x"0009";
	   else
		   EXT_RAM_Q <= x"0000";
	   end if;
	END PROCESS;
	
	B_CTRL : PROCESS
	BEGIN
		wait for 8 ns;
		addr_b <= STD_LOGIC_VECTOR(unsigned(addr_b) + 1);
	END PROCESS;
	
	A_CTRL : PROCESS
	BEGIN
		wait for 8 ns;
		addr_a <= STD_LOGIC_VECTOR(unsigned(addr_a) - 1);
	END PROCESS;
	
	WRITE_A : PROCESS
	BEGIN		   
		data_a <= x"1234";
		wait for 16 ns;
		we_a <= '1';
		wait for 8 ns;
		we_a <= '0';
		wait;
	END PROCESS;		
   
-- *** End Test Bench - User Defined Section ***

END;
