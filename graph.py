def create_and_save_graph(successful_count, failed_count):
    labels = ['Successful', 'Failed']
    values = [successful_count, failed_count]

    plt.bar(labels, values)
    plt.xlabel('Status')
    plt.ylabel('Occurrences')
    plt.title('The history of successful vs failed attempts')

    plt.savefig('status_graph.png', dpi=300)
    plt.close()

def main():
    successful_count, failed_count = count_words('log.txt')
    create_and_save_graph(successful_count, failed_count)

if __name__ == '__main__':
    main()