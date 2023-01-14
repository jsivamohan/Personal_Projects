This project served mainly as practice modeling the data science method in terms of importing, cleaning, transforming, and then analyzing data through the lens of machine learning models.

This project used the python pandas, numpy, and scikit learn packages to try to predict the survival rates on the titanic.

Model 1 was the first attempt to model the data efficiently with a simple linear regression model, basic data transformers like a OneHotEncoder and StandardScaler were used in order to regularize the data, along with an imputer to handle the null values. This model provided less than optimal results so I moved on to model 2.

Model 2 was much more sophisticated, using a variety of machine learning models in order to try to capture the more hidden patterns of the data set hopefully without overfitting the data. A stochastic gradient descent classifier, support vector machine, and RandomForest Classifier were used as the basis for an ensemble generator in which the consensus pick of the models was chosen as the classification. 

The main optimization method implemented was grid search CV to analyze the hyperparameter space and this combined to create a model with a much improved 80 percent accuracy.
