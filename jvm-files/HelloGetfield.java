public class HelloGetfield {
	int cool;
	public static void main(String[] args) {
		HelloGetfield f = new HelloGetfield();

		f.cool = 5;
		int not_cool = f.cool;
	}
}
